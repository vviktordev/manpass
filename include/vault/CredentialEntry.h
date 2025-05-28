//
// Created by Andrzej Machnik on 27/05/2025.
//

// Directory: include/vault/CredentialEntry.h
#ifndef VAULT_CREDENTIALENTRY_H
#define VAULT_CREDENTIALENTRY_H

#include "Entry.h"

namespace vault {

// Represents a credential entry with username and password
class CredentialEntry : public Entry {
public:
    CredentialEntry(const std::string& username, const std::string& password);

    EntryType getType() const override;
    const std::string& getUsername() const;
    const std::string& getPassword() const;

private:
    std::string username;
    std::string password;
};

} // vault

#endif //VAULT_CREDENTIALENTRY_H
