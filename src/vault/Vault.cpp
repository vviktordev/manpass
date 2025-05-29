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

    Folder& Vault::getFolder(const std::string& folderName) {
        auto it = folders.find(folderName);
        if (it == folders.end()) {
            throw std::out_of_range("Folder with name " + folderName + " does not exist in vault " + vaultName);
        }
        return *it->second; // This is safe because we checked for existence above
    }

    const Folder& Vault::getFolder(const std::string& folderName) const {
        auto it = folders.find(folderName);
        if (it == folders.end()) {
            throw std::out_of_range("Folder with name " + folderName + " does not exist in vault " + vaultName);
        }
        return *it->second; // This is safe because we checked for existence above
    }

    Entry& Vault::getEntry(const std::string& folderName, const std::string& entryName) {
        return getFolder(folderName).getEntry(entryName); // reusing already written code in Folder.cpp
    }

    const Entry& Vault::getEntry(const std::string& folderName, const std::string& entryName) const {
        return getFolder(folderName).getEntry(entryName); // reusing already written code in Folder.cpp
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