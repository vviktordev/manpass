//
// Created by wiktor on 5/19/25.
//

// include/Storage.h
#ifndef STORAGE_H
#define STORAGE_H

#include <string>
#include <filesystem>
#include <fstream>
#include <vault/Vault.h>
#include <crypto/Cryptography.h>
#include <json/json.hpp>

using json = nlohmann::json;

namespace storage {

// Manage saving and loading Vaults to disk with encryption
class Storage {
public:
    // Constructs a Storage manager using the given directory (relative to executable)
    explicit Storage(const std::filesystem::path& directory = "vaults");

    // Saves the given vault to a JSON file encrypted with masterPassword
    // Throws on I/O or encryption errors
    void saveVault(const vault::Vault& vault, const std::string& masterPassword) const;

    // Loads and decrypts the vault with the given name using masterPassword
    // Throws on I/O, JSON parse, or decryption errors
    vault::Vault loadVault(const std::string& vaultName, const std::string& masterPassword) const;

private:
    std::filesystem::path vaultsDir; // Directory where vault files are stored
};

} // namespace storage


#endif //STORAGE_H