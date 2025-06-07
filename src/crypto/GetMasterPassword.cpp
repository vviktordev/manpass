//
// Created by wiktor on 6/7/25.
//

#include "crypto/GetMasterPassword.h"

#include <iostream>
#include <stdexcept>
#include <unistd.h>

namespace cryptography {
    // Global flag for signal handler to indicate interruption (Ctrl+C)
    volatile sig_atomic_t gInterruptedFlag = 0;
    void interruptSignalHandler(int signalNumber);

    TerminalSettingsManager::TerminalSettingsManager(int fd_val) : fd(fd_val), settingsChanged(false) {
        if (!isatty(fd)) {
            throw std::runtime_error("Not a terminal");
        }

        if (tcgetattr(fd, &originalSettings) != 0) {
            // If we can't restore the settings then we don't want to change them
            throw std::runtime_error("TerminalSettingsManager: tcgetattr failed to get original settings");
        }
    }

    TerminalSettingsManager::~TerminalSettingsManager() {
        if (settingsChanged) {
            tcsetattr(fd, TCSANOW, &originalSettings);
        }
    }

    bool TerminalSettingsManager::setPasswordMode() {
        struct termios newSettings = originalSettings;

        // Disable echo so that typed characters aren't displayed
        newSettings.c_lflag &= ~ECHO;
        // Disable ICANON (Canonical Mode) so that input is available character by character instead of line by line (needed to display asterisks *)
        newSettings.c_lflag &= ~ICANON;

        if (tcsetattr(fd, TCSANOW, &newSettings) != 0) {
            return false;
        }

        settingsChanged = true;
        return true;
    }

    void interruptSignalHandler(int signalNumber) {
        gInterruptedFlag = 1;
    }

    Botan::secure_vector<char> getMasterPassword() {
        // Using non-interactive input methods (e.g. piping from a file) is usually insecure
        if (!isatty(STDIN_FILENO)) {
            throw std::runtime_error("Reading password from non-interactive input is not allowed.");
        }

        TerminalSettingsManager termManager(STDIN_FILENO);
        if (!termManager.setPasswordMode()) {
            throw std::runtime_error("Failed to set terminal for secure password input.");
        }

        // Set up a signal handler for SIGINT (Ctrl+C)
        struct sigaction oldSigintAction, newSigintAction;
        memset(&newSigintAction, 0, sizeof(newSigintAction));
        newSigintAction.sa_handler = interruptSignalHandler;
        sigemptyset(&newSigintAction.sa_mask); // Block no other signals during handler
        if (sigaction(SIGINT, &newSigintAction, &oldSigintAction) != 0) {
            perror("sigaction (set SIGINT handler)");
        }

        std::cout << "Master password: " << std::flush;
        Botan::secure_vector<char> password;

        char ch;
        while (true) {
            if (gInterruptedFlag) break;

            // Read one character from stdin
            ssize_t bytesRead = read(STDIN_FILENO, &ch, 1);

            // read is blocking so check for interrupt again
            if (gInterruptedFlag) break;

            if (bytesRead < 0) {
                if (errno == EINTR) continue; // Interrupted by a signal
                throw std::runtime_error("A read error occured when reading a character for the password.");
            }

            // EOF
            if (bytesRead == 0) {
                std::cout << std::endl;
                break;
            }

            if (ch == '\n' || ch == '\r') { // Enter key
                std::cout << std::endl;
                break;
            } else if (ch == 127 || ch == '\b') { // Backspace key
                if (!password.empty()) {
                    password.pop_back();
                    // Visual feedback for backspace: move cursor left, print space, move cursor left again
                    write(STDOUT_FILENO, "\b \b", 3);
                }
            } else if (isprint(static_cast<unsigned char>(ch))) { // Printable character
                password.push_back(ch);
                write(STDOUT_FILENO, "*", 1);
            }
        }

        // Restore the original SIGINT signal handler
        if (sigaction(SIGINT, &oldSigintAction, nullptr) != 0) {
            perror("sigaction (restore SIGINT handler)");
        }

        if (gInterruptedFlag) {
            gInterruptedFlag = 0; // Reset flag for any subsequent calls
            throw std::runtime_error("Password input interrupted by user.");
        }

        return password;
    }
}
