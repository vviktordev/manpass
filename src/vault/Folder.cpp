//
// Created by Andrzej Machnik on 27/05/2025.
//

// Directory: src/vault/Folder.cpp
#include "vault/Folder.h"

namespace vault {

    Folder::Folder(const std::string& fnm) : folderName(fnm) {}

    void Folder::addEntry(std::unique_ptr<Entry> entry, const std::string& entryName) {
        if (entryExists(entryName)) {
            throw std::runtime_error("Entry with name " + entryName + " already exists in folder " + folderName);
        }
        entries[entryName] = std::move(entry);
    }

    void Folder::deleteEntry(const std::string &entryName) {
        if (!entryExists(entryName))
            return;
        entries.erase(entryName);
    }

    Entry& Folder::getEntry(const std::string& entryName) {
        auto it = entries.find(entryName);
        if (it == entries.end()) {
            throw std::out_of_range("Entry with name " + entryName + " does not exist in folder " + folderName);
        }
        return *it->second.get(); // This is safe because we checked for existence above
    }

    const Entry& Folder::getEntry(const std::string& entryName) const {
        auto it = entries.find(entryName);
        if (it == entries.end()) {
            throw std::out_of_range("Entry with name " + entryName + " does not exist in folder " + folderName);
        }
        return *it->second.get(); // This is safe because we checked for existence above
    }

    std::vector<const Entry*> Folder::getAllEntries() const {
        std::vector<const Entry*> result;
        for (const auto& [name, entry] : entries) {
            result.push_back(entry.get());
        }
        return result;
    }

    std::vector<std::string> Folder::getEntryNames() const {
        std::vector<std::string> names;
        for (const auto& [name, entry] : entries) {
            names.push_back(name);
        }
        return names;
    }

    const std::string& Folder::getName() const {
        return folderName;
    }

    void Folder::setName(std::string name) {
        folderName = name;
    }

    bool Folder::entryExists(const std::string& entryName) const {
        return entries.find(entryName) != entries.end();
    }

    Folder::Folder(Folder && other) noexcept :
    folderName(std::move(other.folderName)), entries(std::move(other.entries)) {}

    Folder& Folder::operator=(Folder && other) noexcept {
        if (this != &other) {
            folderName = std::move(other.folderName);
            entries = std::move(other.entries);
        }
        return *this;
    }

} // namespace vault