//
// Created by wiktor on 5/19/25.
//

#include "Storage.h"

#include <utility>

namespace storage {

    Storage::Storage(const std::filesystem::path& directory) : vaultsDir(directory) {
        std::error_code ec;
        if (!std::filesystem::exists(vaultsDir, ec)) {
            if (!std::filesystem::create_directories(vaultsDir, ec) && ec) {
                throw std::runtime_error("Failed to create vaults directory: " + ec.message());
            }
        }
    }

    void Storage::saveVault(const vault::Vault& vault, Botan::secure_vector<char> masterPassword) const {
        // Encrypt vault data
        json serializedVault = vault;
        cryptography::EncryptedBlob blob = cryptography::encrypt(
            serializedVault.dump(),
            std::move(masterPassword),
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

    vault::Vault Storage::loadVault(const std::string &vaultName, Botan::secure_vector<char> masterPassword) const {
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
        std::string stringSerializedVault = cryptography::decrypt(blob, std::move(masterPassword));
        json serializedVault = json::parse(stringSerializedVault);

        vault::Vault vault("");
        from_json(serializedVault, vault);

        vault.cryptoAlgorithm = blob.algorithm;
        vault.cryptoKDF = blob.kdf;
        vault.cryptoKDFIterations = blob.kdfIterations;
        vault.cryptoBase64Salt = blob.base64Salt;

        return vault;
    }

    fs::path getDefaultVaultsDirectory() {
        fs::path dataDir;

    #ifdef _WIN32
        // Try LOCALAPPDATA
        char* localAppDataPath = nullptr;
        size_t len;
        errno_t err = _dupenv_s(&localAppDataPath, &len, "LOCALAPPDATA");

        if (err == 0 && localAppDataPath != nullptr) {
            dataDir = fs::path(localAppDataPath);
            free(localAppDataPath); // Important to free memory allocated by _dupenv_s
        } else {
            char appDataPathWin[MAX_PATH];
            if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, appDataPathWin))) {
                 dataDir = fs::path(appDataPathWin);
            } else {
                throw std::runtime_error("Could not determine AppData directory on Windows.");
            }
        }
        if (dataDir.empty()) {
             throw std::runtime_error("Could not determine a valid data directory on Windows.");
        }

    #elif defined(__APPLE__)
        const char* homeDir = getenv("HOME");
        if (homeDir) {
            dataDir = fs::path(homeDir) / "Library" / "Application Support";
        } else {
            throw std::runtime_error("Could not determine HOME directory on macOS.");
        }
    #else // Assuming Linux/other Unix-like
        const char* xdgDataHome = getenv("XDG_DATA_HOME");
        if (xdgDataHome && xdgDataHome[0] != '\0') {
            dataDir = fs::path(xdgDataHome);
        } else {
            const char* homeDir = getenv("HOME");
            if (homeDir) {
                dataDir = fs::path(homeDir) / ".local" / "share";
            } else {
                throw std::runtime_error("Could not determine XDG_DATA_HOME or HOME directory on Linux.");
            }
        }
    #endif

        if (dataDir.empty()) {
            throw std::runtime_error("Failed to determine a default application data directory.");
        }

        return dataDir / "manpass" / "vaults";
    }
} // namespace storage