//
// Created by Andrzej Machnik on 27/05/2025.
//

// Directory: include/vault/Folder.h
#ifndef VAULT_FOLDER_H
#define VAULT_FOLDER_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <stdexcept>
#include "Entry.h"

using json = nlohmann::json;

namespace vault {

class Folder {
public:
    explicit Folder(const std::string& folderName);

    // Adds an entry to the folder by name (throws if entry already exists)
    // Entries are uniquely owned by a Folder, so we use unique_ptr
    void addEntry(std::unique_ptr<Entry> entry, const std::string& entryName);

    // Retrieves an entry by name (mutable and immutable versions)
    Entry& getEntry(const std::string& entryName);
    const Entry& getEntry(const std::string& entryName) const;

    std::vector<const Entry*> getAllEntries() const; // Gets all entry pointers
    std::vector<std::string> getEntryNames() const; // Gets names of all entries

    const std::string& getName() const; // Return folder name

    // Helper to check if entry exists
    bool entryExists(const std::string& entryName) const;

    // Explicit disallowance of copy operations and allowance for moving operations is required due to unique_ptr use
    // Delete copy operations
    Folder(const Folder&) = delete;
    Folder& operator=(const Folder&) = delete;
    // Enable move operations
    Folder(Folder&&) noexcept;
    Folder& operator=(Folder&&) noexcept;

    friend void to_json(json& j, const Folder& folder);

private:
    std::string folderName;

    // We use unique_ptr to enforce exclusive ownership of each entry by the folder.
    // It also helps store entries of different kinds and improves memory management.
    std::unordered_map<std::string, std::unique_ptr<Entry>> entries; // Map of entry name to Entry object
};

void from_json(const json& j, Folder& folder);


} // vault

#endif //VAULT_FOLDER_H
