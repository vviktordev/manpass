//
// Created by Andrzej Machnik on 27/05/2025.
//

// Directory: src/vault/Vault.cpp
#include "vault/Vault.h"

namespace vault {

    Vault::Vault(const std::string& vnm) : vaultName(vnm) {}


    void Vault::addFolder(std::unique_ptr<Folder> folder) {
        const std::string& name = folder->getName();
        if (folderExists(name)) {
            throw std::runtime_error("Folder with name " + name + " already exists in vault " + vaultName);
        }
        folders[name] = std::move(folder);
    }

    Folder* Vault::getFolder(const std::string& folderName) {
        auto it = folders.find(folderName);
        return it != folders.end() ? it->second.get() : nullptr;
    }

    const Folder* Vault::getFolder(const std::string& folderName) const {
        auto it = folders.find(folderName);
        return it != folders.end() ? it->second.get() : nullptr;
    }

    Entry* Vault::getEntry(const std::string& folderName, const std::string& entryName) {
        Folder* folder = getFolder(folderName);
        return folder ? folder->getEntry(entryName) : nullptr;
    }

    const Entry* Vault::getEntry(const std::string& folderName, const std::string& entryName) const {
        const Folder* folder = getFolder(folderName);
        return folder ? folder->getEntry(entryName) : nullptr;
    }

    std::vector<const Folder*> Vault::getAllFolders() const {
        std::vector<const Folder*> result;
        for (const auto& [name, folder] : folders) {
            result.push_back(folder.get());
        }
        return result;
    }

    std::vector<std::string> Vault::getFolderNames() const {
        std::vector<std::string> names;
        for (const auto& [name, folder] : folders) {
            names.push_back(name);
        }
        return names;
    }

    const std::string& Vault::getName() const {
        return vaultName;
    }

    bool Vault::folderExists(const std::string& folderName) const {
        return folders.find(folderName) != folders.end();
    }

    Vault::Vault(Vault && other) noexcept :
    vaultName(std::move(other.vaultName)),
    folders(std::move(other.folders)) {}

    Vault &Vault::operator=(Vault && other) noexcept {
        if (this != &other) {
            vaultName = std::move(other.vaultName);
            folders = std::move(other.folders);
        }
        return *this;
    }


} // namespace vault