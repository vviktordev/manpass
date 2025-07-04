//
// Created by Andrzej Machnik on 27/05/2025.
//

// Directory: include/vault/Vault.h
#ifndef VAULT_H
#define VAULT_H

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include "Folder.h"
#include "Entry.h"

namespace vault {

class Vault {
public:
    // Metadata required for encrypting the vault
    std::string cryptoAlgorithm;
    std::string cryptoKDF;
    int cryptoKDFIterations;
    std::string cryptoBase64Salt;

    explicit Vault(const std::string& vaultName);

    // Adds a folder to the vault (throws if folder already exists)
    void addFolder(std::unique_ptr<Folder> folderName);

    void deleteFolder(const std::string& folderName);

    // Shorthand for adding an entry to a folder
    void addEntry(const std::string& folderName, const std::string& entryName, std::unique_ptr<Entry> entry);

    // Retrieves a folder by name (mutable and immutable versions)
    Folder& getFolder(const std::string& folderName);
    const Folder& getFolder(const std::string& folderName) const;

    // Retrieves a specific entry by folder and entry name (mutable and immutable versions)
    Entry& getEntry(const std::string& folderName, const std::string& entryName);
    const Entry& getEntry(const std::string& folderName, const std::string& entryName) const;

    std::vector<const Folder*> getAllFolders() const; // Gets all folder pointers
    std::vector<std::string> getFolderNames() const; // Gets names of all folders

    const std::string& getName() const; // Returns vault name
    void setName(std::string name);

    // Helper to check if folder exists
    bool folderExists(const std::string& folderName) const;

    // Helper to check if entry exists
    bool entryExists(const std::string& folderName, const std::string& entryName) const;

    // This has to be done in the Vault object because in addition to just updating the Folder's property, we also need to update the 'folders' unordered map
    void changeFolderName(const std::string& oldName, const std::string& newName);

    // Explicit disallowance of copy operations and allowance for moving operations is required due to unique_ptr use
    // Disable copy operations to enforce unique ownership
    Vault(const Vault&) = delete;
    Vault& operator=(const Vault&) = delete;
    // Enable move operations
    Vault(Vault&&) noexcept;
    Vault& operator=(Vault&&) noexcept;

    friend void to_json(json& j, const Vault& vault);

private:
    std::string vaultName; // Name of the vault
    std::unordered_map<std::string, std::unique_ptr<Folder>> folders; // Map of folder name to Folder objects

};

void from_json(const json& j, Vault& vault);


} // vault

#endif //VAULT_H
