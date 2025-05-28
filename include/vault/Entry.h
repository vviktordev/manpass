//
// Created by Andrzej Machnik on 27/05/2025.
//

// Directory: inlclude/vault/Entry.h
#ifndef VAULT_ENTRY_H
#define VAULT_ENTRY_H

#include <string>

namespace vault {

// Enum used to distinguish between types of entries
enum class EntryType {
    CREDENTIAL,
    NOTE,
};

// Abstrac base class representing a generic entry
class Entry {
public:
    virtual ~Entry() = default;
    virtual EntryType getType() const = 0; // Must be implemented by derived classes
};

} // vault

#endif //VAULT_ENTRY_H
