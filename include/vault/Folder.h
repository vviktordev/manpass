//
// Created by Andrzej Machnik on 27/05/2025.
//

// Directory: include/vault/Folder.h
#ifndef VAULT_FOLDER_H
#define VAULT_FOLDER_H

#include <string>
#include <vector>
#include <memory>
#include "Entry.h"

namespace vault {

class Folder {
public:
    Folder(const std::string& folderName);

    // Entries are uniquely owned by a Folder, so we use unique_ptr
    void addEntry(std::unique_ptr<Entry> entry);
    const std::vector<std::unique_ptr<Entry>>& getEntries() const;
    const std::string& getName() const;

    // Explicitly delete copy constructor and copy assignment - prevents accidental copying of non-copyable resources
    Folder(const Folder&) = delete;
    Folder& operator=(const Folder&) = delete;

    // Allow move construction and assignment
    Folder(Folder&&) noexcept;
    Folder& operator=(Folder&&) noexcept;

private:
    std::string folderName;

    // We use unique_ptr to enforce exclusive ownership of each entry by the folder.
    // It also helps store entries of different kinds and improves memory management.
    std::vector<std::unique_ptr<Entry>> entries;
};

} // vault

#endif //VAULT_FOLDER_H
