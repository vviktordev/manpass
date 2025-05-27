//
// Created by Andrzej Machnik on 27/05/2025.
//

// Directory: src/vault/Folder.cpp
#include "vault/Folder.h"

namespace vault {

    Folder::Folder(const std::string& fnm) : folderName(fnm) {}

    void Folder::addEntry(std::unique_ptr<Entry> entry) {
        entries.push_back(std::move(entry));
    }

    const std::vector<std::unique_ptr<Entry>>& Folder::getEntries() const {
        return entries;
    }

    const std::string& Folder::getName() const {
        return folderName;
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