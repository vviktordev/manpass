//
// Created by wiktor on 5/19/25.
//

#include "Storage.h"

#include <unistd.h>

namespace storage {

    Storage::Storage(const std::filesystem::path& directory) : vaultsDir(directory) {
        std::error_code ec;
        if (!std::filesystem::exists(vaultsDir, ec)) {
            if (!std::filesystem::create_directories(vaultsDir, ec) && ec) {
                throw std::runtime_error("Failed to create vaults directory: " + ec.message());
            }
        }
    }

    void Storage::saveVault(const vault::Vault& vault, const std::string& masterPassword) const {
        // Encrypt vault data
        cryptography::EncryptedBlob blob = cryptography::encryptVault(vault, masterPassword);

        // Build JSON wrapper
        json j;
        j["Algorithm"] = blob.algorithm;
        j["KDF"]       = blob.kdf;
        j["Salt"]      = blob.salt;
        j["Data"]      = blob.data;

        // Write to file
        std::filesystem::path filePath = vaultsDir / (vault.getName() + ".json");
        std::ofstream ofs(filePath);
        if (!ofs)
            throw std::runtime_error("Failed to open file for writing: " + filePath.string());
        ofs << j.dump(4);
    }

    vault::Vault Storage::loadVault(const std::string &vaultName, const std::string &masterPassword) const {
        // Read file
        std::filesystem::path filePath = vaultsDir / (vaultName + ".json");
        std::ifstream ifs(filePath);
        if (!ifs)
            throw std::runtime_error("Failed to open file for reading: " + filePath.string());

        json j;
        ifs >> j;

        // Extract EncryptedBlob
        cryptography::EncryptedBlob blob;
        blob.algorithm = j["Algorithm"].get<std::string>();
        blob.kdf = j["KDF"].get<std::string>();
        blob.salt = j["Salt"].get<std::string>();
        blob.data = j["Data"].get<std::string>();

        // Decrypt and return
        return cryptography::decryptVault(blob, masterPassword);
    }


} // namespace storage