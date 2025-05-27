//
// Created by Andrzej Machnik on 27/05/2025.
//

// Directory: include/vault/Vault.h
#ifndef VAULT_H
#define VAULT_H

#include <string>
#include "Folder.h"

namespace vault {

class Vault {
public:
    Vault(const std::string& vaultName, const std::string& masterPasswordHash);

    bool unlock(std::string& inputPasswordHash) const;

    // Accept folder by rvalue reference to enable moving (since copying is not allowed)
    void addFolder(std::unique_ptr<Folder> folderName);
    const std::vector<std::unique_ptr<Folder>>& getFolders() const;

    // Explicitly delete copy constructor and copy assignment - prevents accidental copying of non-copyable resources
    Vault(const Vault&) = delete;
    Vault& operator=(const Vault&) = delete;

    // Allow move construction and assignment
    Vault(Vault&&) noexcept;
    Vault& operator=(Vault&&) noexcept;

private:
    std::string vaultName;
    std::string masterPasswordHash;
    std::vector<std::unique_ptr<Folder>> folders;

};

} // vault

#endif //VAULT_H
