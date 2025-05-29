//
// Created by Andrzej Machnik on 27/05/2025.
//

//Directory: include/vault/NoteEntry.h
#ifndef VAULT_NOTEENTRY_H
#define VAULT_NOTEENTRY_H

#include "Entry.h"
#include "json/json.hpp"

using json = nlohmann::json;

namespace vault {

// Reperesents a simple note entry
class NoteEntry : public Entry {
public:
    NoteEntry(const std::string& noteText);

    EntryType getType() const override;
    const std::string& getNoteText() const;

    friend void to_json(json& j, const NoteEntry& entry);

private:
    std::string noteText;
};

void from_json(const json& j, NoteEntry& entry);

} // vault

#endif //VAULT_NOTEENTRY_H
