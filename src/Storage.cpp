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
        json serializedVault = vault;
        cryptography::EncryptedBlob blob = cryptography::encrypt(
            serializedVault.dump(),
            masterPassword,
            vault.cryptoAlgorithm,
            vault.cryptoKDF,
            vault.cryptoBase64Salt,
            vault.cryptoKDFIterations
        );

        // Build JSON wrapper
        json j;
        j["Algorithm"]     = blob.algorithm;
        j["KDF"]           = blob.kdf;
        j["KDFIterations"] = blob.kdfIterations;
        j["Salt"]          = blob.base64Salt;
        j["Nonce"]         = blob.base64Nonce;
        j["Data"]          = blob.base64Ciphertext;

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
        blob.kdfIterations = j["KDFIterations"].get<int>();
        blob.base64Salt = j["Salt"].get<std::string>();
        blob.base64Nonce = j["Nonce"].get<std::string>();
        blob.base64Ciphertext = j["Data"].get<std::string>();

        // Decrypt and return
        std::string stringSerializedVault = cryptography::decrypt(blob, masterPassword);
        json serializedVault = json::parse(stringSerializedVault);

        vault::Vault vault("");
        from_json(serializedVault, vault);

        vault.cryptoAlgorithm = blob.algorithm;
        vault.cryptoKDF = blob.kdf;
        vault.cryptoKDFIterations = blob.kdfIterations;
        vault.cryptoBase64Salt = blob.base64Salt;

        return vault;
    }


} // namespace storage