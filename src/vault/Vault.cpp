//
// Created by Andrzej Machnik on 27/05/2025.
//

// Directory: src/vault/Vault.cpp
#include "vault/Vault.h"

namespace vault {

    Vault::Vault(const std::string& vnm, const std::string& mph) : vaultName(vnm), masterPasswordHash(mph) {}

    bool Vault::unlock(std::string& inputPasswordHash) const {
        return inputPasswordHash == masterPasswordHash;
    }

    void Vault::addFolder(std::unique_ptr<Folder> folder) {
        folders.push_back(std::move(folder));
    }

    const std::vector<std::unique_ptr<Folder>>& Vault::getFolders() const {
        return folders;
    }

    Vault::Vault(Vault && other) noexcept :
    masterPasswordHash(std::move(other.masterPasswordHash)),
    vaultName(std::move(other.vaultName)),
    folders(std::move(other.folders)) {}

    Vault &Vault::operator=(Vault && other) noexcept {
        if (this != &other) {
            masterPasswordHash = std::move(other.masterPasswordHash);
            vaultName = std::move(other.vaultName);
            folders = std::move(other.folders);
        }
        return *this;
    }


} // namespace vault