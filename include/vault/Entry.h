//
// Created by Andrzej Machnik on 27/05/2025.
//

// Directory: inlclude/vault/Entry.h
#ifndef VAULT_ENTRY_H
#define VAULT_ENTRY_H

#include <string>
#include <json/json.hpp>

using json = nlohmann::json;

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

    friend void to_json(json& j, const Entry& entry);

};

std::unique_ptr<Entry> parseEntry(const json &j);


} // vault

#endif //VAULT_ENTRY_H
