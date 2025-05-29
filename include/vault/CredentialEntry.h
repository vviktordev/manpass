//
// Created by Andrzej Machnik on 27/05/2025.
//

// Directory: include/vault/CredentialEntry.h
#ifndef VAULT_CREDENTIALENTRY_H
#define VAULT_CREDENTIALENTRY_H

#include "Entry.h"
#include "json/json.hpp"

using json = nlohmann::json;

namespace vault {

// Represents a credential entry with username and password
class CredentialEntry : public Entry {
public:
    CredentialEntry(const std::string& username, const std::string& password);

    EntryType getType() const override;
    const std::string& getUsername() const;
    const std::string& getPassword() const;

    friend void to_json(json& j, const CredentialEntry& entry);

private:
    std::string username;
    std::string password;
};

void from_json(const json& j, CredentialEntry& entry);

} // vault

#endif //VAULT_CREDENTIALENTRY_H
