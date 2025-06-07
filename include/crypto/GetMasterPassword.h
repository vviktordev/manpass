//
// Created by wiktor on 6/7/25.
//

#ifndef GETMASTERPASSWORD_H
#define GETMASTERPASSWORD_H

#include <csignal>
#include <termios.h>
#include <botan/secmem.h>

namespace cryptography {
    // Change the settings of the terminal using RAII. This way we are certain the settings get restored correctly when the object goes out of scope
    class TerminalSettingsManager {
    public:
        TerminalSettingsManager(int fd);
        ~TerminalSettingsManager();
        bool setPasswordMode();
    private:
        int fd;
        bool settingsChanged;
        struct termios originalSettings;
    };

    Botan::secure_vector<char> getMasterPassword();
}

#endif //GETMASTERPASSWORD_H