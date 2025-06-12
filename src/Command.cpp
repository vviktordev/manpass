//
// Created by wiktor on 6/7/25.
//

#include "Command.h"

#include <iostream>
#include "crypto/GetMasterPassword.h"
#include "vault/CredentialEntry.h"
#include "vault/NoteEntry.h"

using namespace cryptography;
using namespace vault;

// Helper function to ask for confirmation (used in Delete commands). Defaults to false
bool askForConfirmation(const std::string& prompt_message) {
    std::string input_str;
    std::cout << prompt_message << " (y/N): ";

    if (!std::getline(std::cin, input_str)) { // Error reading input (e.g., EOF from Ctrl+D)
        std::cout << "Error reading input, defaulting to no" << std::endl;
        return false;
    }

    // Trim leading/trailing whitespace
    input_str.erase(0, input_str.find_first_not_of(" \t\n\r\f\v"));
    input_str.erase(input_str.find_last_not_of(" \t\n\r\f\v") + 1);

    if (input_str.empty()) {
        // User just pressed Enter
        return false;
    }

    // Convert to lowercase for case-insensitive comparison
    std::transform(input_str.begin(), input_str.end(), input_str.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    if (input_str == "y" || input_str == "yes") {
        return true;
    } else if (input_str == "n" || input_str == "no") {
        return false;
    } else {
        std::cout << "Invalid input. Please enter 'y', 'yes', 'n', or 'no'" << std::endl;
        return askForConfirmation(prompt_message);
    }
}


Command::~Command() = default;


// --- ADD VAULT ---
AddVaultCommand::AddVaultCommand(std::string vaultName, Storage& storage) : vaultName(vaultName), storage(storage) {}

void AddVaultCommand::execute() {
    if (storage.vaultExists(vaultName))
        throw std::runtime_error("Vault already exists");
    std::cout << "Adding vault \"" << vaultName << "\"" << std::endl;
    Vault vault(vaultName);
    Botan::secure_vector<char> masterPassword = getMasterPassword();
    storage.saveVault(vault, masterPassword);
}


// --- ADD FOLDER ---
AddFolderCommand::AddFolderCommand(std::string vaultName, std::string folderName, Storage &storage) : vaultName(vaultName), folderName(folderName), storage(storage) {}

void AddFolderCommand::execute() {
    std::cout << "Adding folder \"" << folderName << "\"" << std::endl;
    Botan::secure_vector<char> masterPassword = getMasterPassword();
    Vault vault = storage.loadVault(vaultName, masterPassword);

    if (vault.folderExists(folderName))
        throw std::runtime_error("Folder already exists");

    auto folder = std::make_unique<Folder>(folderName);
    vault.addFolder(std::move(folder));
    storage.saveVault(vault, masterPassword);
}


// --- ADD CREDENTIAL ---
AddCredentialCommand::AddCredentialCommand(std::string vault, std::string folder, std::string credential, Storage& storage) :
    vaultName(vault), folderName(folder), credentialName(credential), storage(storage) {}

void AddCredentialCommand::execute() {
    std::cout << "Adding credential \"" << credentialName << "\"" << std::endl;
    Botan::secure_vector<char> masterPassword = getMasterPassword();
    Vault vault = storage.loadVault(vaultName, masterPassword);

    if (vault.entryExists(folderName, credentialName))
        throw std::runtime_error("An entry with this name already exists");

    std::string username;
    std::cout << "Username: ";
    std::getline(std::cin, username);

    std::string password;
    std::cout << "Password: ";
    std::getline(std::cin, password);

    auto entry = std::make_unique<CredentialEntry>(username, password);
    vault.addEntry(folderName, credentialName, std::move(entry));

    storage.saveVault(vault, masterPassword);
}


// --- ADD NOTE ---
AddNoteCommand::AddNoteCommand(std::string vaultName, std::string folderName, std::string noteName, Storage &storage) :
    vaultName(vaultName), folderName(folderName), noteName(noteName), storage(storage) {}

void AddNoteCommand::execute() {
    std::cout << "Adding note \"" << noteName << "\"" << std::endl;
    Botan::secure_vector<char> masterPassword = getMasterPassword();
    Vault vault = storage.loadVault(vaultName, masterPassword);

    if (vault.entryExists(folderName, noteName))
        throw std::runtime_error("An entry with this name already exists");

    std::string text;
    std::cout << "Note contents: ";
    std::getline(std::cin, text);

    auto entry = std::make_unique<NoteEntry>(text);
    vault.addEntry(folderName, noteName, std::move(entry));

    storage.saveVault(vault, masterPassword);
}


// --- SHOW (ALL VAULTS) ---
ShowCommand::ShowCommand(Storage &storage) : storage(storage) {}

void ShowCommand::execute() {
    std::vector<std::string> vaultNames = storage.getAllVaultNames();

    if (vaultNames.empty()) {
        std::cout << "No vaults found" << std::endl;
        return;
    }

    for (const std::string& name : vaultNames) {
        std::cout << name << std::endl;
    }
}


// --- SHOW VAULT ---
ShowVaultCommand::ShowVaultCommand(std::string vaultName, Storage &storage) : vaultName(vaultName), storage(storage) {}

void ShowVaultCommand::execute() {
    if (!storage.vaultExists(vaultName))
        throw std::runtime_error("Vault doesn't exist");

    Botan::secure_vector<char> masterPassword = getMasterPassword();
    Vault vault = storage.loadVault(vaultName, masterPassword);

    std::vector<const Folder*> folders = vault.getAllFolders();
    for (int i = 0; i < folders.size(); i++) {
        const Folder* folder = folders.at(i);
        std::cout << "/" << folder->getName() << std::endl;

        std::vector<std::string> entriesNames = folder->getEntryNames();
        for (int j = 0; j < entriesNames.size(); j++) {
            std::cout << "  " << entriesNames.at(j) << std::endl;
        }
    }
}


// --- SHOW FOLDER ---
ShowFolderCommand::ShowFolderCommand(std::string vaultName, std::string folderName, Storage &storage) : vaultName(vaultName), folderName(folderName), storage(storage) {}

void ShowFolderCommand::execute() {
    Botan::secure_vector<char> masterPassword = getMasterPassword();
    Vault vault = storage.loadVault(vaultName, masterPassword);
    const Folder& folder = vault.getFolder(folderName);
    std::vector<std::string> entriesNames = folder.getEntryNames();

    for (int i = 0; i < entriesNames.size(); i++) {
        std::string entryName = entriesNames.at(i);
        EntryType entryType = folder.getEntry(entryName).getType();

        std::string type;
        switch (entryType) {
            case EntryType::CREDENTIAL: {
                type = "credential";
                break;
            }
            case EntryType::NOTE: {
                type = "note";
                break;
            }
        }

        std::cout << entryName << " (" << type << ")" << std::endl;
    }
}


// --- SHOW ENTRY ---
ShowEntryCommand::ShowEntryCommand(std::string vaultName, std::string folderName, std::string entryName, Storage &storage) :
    vaultName(vaultName), folderName(folderName), entryName(entryName), storage(storage) {}

void ShowEntryCommand::execute() {
    Botan::secure_vector<char> masterPassword = getMasterPassword();
    Vault vault = storage.loadVault(vaultName, masterPassword);

    Entry& entry = vault.getEntry(folderName, entryName);
    switch (entry.getType()) {
        case EntryType::CREDENTIAL: {
            auto credential = dynamic_cast<CredentialEntry&>(entry);
            std::cout << "Username: " << credential.getUsername() << std::endl;
            std::cout << "Password: " << credential.getPassword() << std::endl;
            break;
        }
        case EntryType::NOTE: {
            auto note = dynamic_cast<NoteEntry&>(entry);
            std::cout << note.getNoteText() << std::endl;
            break;
        }
    }
}


// --- UPDATE VAULT ---
UpdateVaultCommand::UpdateVaultCommand(std::string vaultName, Storage &storage) : vaultName(vaultName), storage(storage) {}

void UpdateVaultCommand::execute() {
    Botan::secure_vector<char> masterPassword = getMasterPassword();
    Vault vault = storage.loadVault(vaultName, masterPassword);

    std::string newVaultName;
    std::cout << "New vault name: ";
    std::getline(std::cin, newVaultName);
    vault.setName(newVaultName);

    std::cout << "New ";
    Botan::secure_vector<char> newMasterPassword = getMasterPassword();

    storage.saveVault(vault, newMasterPassword);
    storage.deleteVault(vaultName);
}


// --- UPDATE FOLDER ---
UpdateFolderCommand::UpdateFolderCommand(std::string vaultName, std::string folderName, Storage &storage) :
    vaultName(vaultName), folderName(folderName), storage(storage) {}

void UpdateFolderCommand::execute() {
    Botan::secure_vector<char> masterPassword = getMasterPassword();
    Vault vault = storage.loadVault(vaultName, masterPassword);

    if (!vault.folderExists(folderName))
        throw std::runtime_error("Folder does not exist");

    std::string newFolderName;
    std::cout << "New folder name: ";
    std::getline(std::cin, newFolderName);

    if (vault.folderExists(newFolderName)) {
        throw std::runtime_error("Folder with name " + newFolderName + " already exists");
    }

    vault.changeFolderName(folderName, newFolderName);
    storage.saveVault(vault, masterPassword);
}


// --- UPDATE ENTRY ---
UpdateEntryCommand::UpdateEntryCommand(std::string vaultName, std::string folderName, std::string entryName, Storage &storage) :
    vaultName(vaultName), folderName(folderName), entryName(entryName), storage(storage) {}

void UpdateEntryCommand::execute() {
    Botan::secure_vector<char> masterPassword = getMasterPassword();
    Vault vault = storage.loadVault(vaultName, masterPassword);

    if (!vault.entryExists(folderName, entryName))
        throw std::runtime_error("Entry does not exist");

    std::string newEntryName;
    std::cout << "New entry name: ";
    std::getline(std::cin, newEntryName);

    if (vault.entryExists(folderName, newEntryName)) {
        throw std::runtime_error("Entry with name " + newEntryName + " already exists");
    }

    Folder& folder = vault.getFolder(folderName);
    Entry& entry = folder.getEntry(entryName);

    switch (entry.getType()) {
        case EntryType::CREDENTIAL: {
            std::string username;
            std::cout << "New username: ";
            std::getline(std::cin, username);

            std::string password;
            std::cout << "New password: ";
            std::getline(std::cin, password);

            auto entry = std::make_unique<CredentialEntry>(username, password);
            folder.addEntry(std::move(entry), newEntryName);
            folder.deleteEntry(entryName);
            break;
        }
        case EntryType::NOTE: {
            std::string text;
            std::cout << "New note contents: ";
            std::getline(std::cin, text);

            auto entry = std::make_unique<NoteEntry>(text);
            folder.addEntry(std::move(entry), newEntryName);
            folder.deleteEntry(entryName);
            break;
        }
    }

    storage.saveVault(vault, masterPassword);
}


// --- DELETE VAULT ---
DeleteVaultCommand::DeleteVaultCommand(std::string vaultName, Storage &storage) : vaultName(vaultName), storage(storage) {}

void DeleteVaultCommand::execute() {
    // Let's pretend that only an authenticated user can delete the vault (even though the vaults are just JSON files stored on disk)
    Botan::secure_vector<char> masterPassword = getMasterPassword();
    Vault vault = storage.loadVault(vaultName, masterPassword);

    bool confirmed = askForConfirmation("Are you sure you want to delete vault '" + vaultName + "' and all of its content?");
    if (!confirmed) return;

    storage.deleteVault(vaultName);
}


// --- DELETE FOLDER ---
DeleteFolderCommand::DeleteFolderCommand(std::string vaultName, std::string folderName, Storage &storage) :
    vaultName(vaultName), folderName(folderName), storage(storage) {}

void DeleteFolderCommand::execute() {
    Botan::secure_vector<char> masterPassword = getMasterPassword();
    Vault vault = storage.loadVault(vaultName, masterPassword);

    if (!vault.folderExists(folderName))
        throw std::runtime_error("Folder with this name does not exist");

    bool confirmed = askForConfirmation("Are you sure you want to delete folder '" + folderName + "' and all of its content?");
    if (!confirmed) return;

    vault.deleteFolder(folderName);
    storage.saveVault(vault, masterPassword);
}


// --- DELETE ENTRY ---
DeleteEntryCommand::DeleteEntryCommand(std::string vaultName, std::string folderName, std::string entryName, Storage &storage) :
    vaultName(vaultName), folderName(folderName), entryName(entryName), storage(storage) {}

void DeleteEntryCommand::execute() {
    Botan::secure_vector<char> masterPassword = getMasterPassword();
    Vault vault = storage.loadVault(vaultName, masterPassword);

    Folder& folder = vault.getFolder(folderName);
    if (!folder.entryExists(entryName))
        throw std::runtime_error("Entry with this name does not exist");

    bool confirmed = askForConfirmation("Are you sure you want to delete entry '" + folderName + "'?");
    if (!confirmed) return;

    folder.deleteEntry(entryName);
    storage.saveVault(vault, masterPassword);
}