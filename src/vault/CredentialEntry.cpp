//
// Created by Andrzej Machnik on 27/05/2025.
//

// Directory: src/vault/CredentialEntry.cpp
#include "vault/CredentialEntry.h"

namespace vault {

    CredentialEntry::CredentialEntry(const std::string& usr, const std::string& pwd) : username(usr), password(pwd) {}

    EntryType CredentialEntry::getType() const {
        return EntryType::CREDENTIAL;
    }

    const std::string& CredentialEntry::getUsername() const {
        return username;
    }

    const std::string& CredentialEntry::getPassword() const {
        return password;
    }

} // namespace vault