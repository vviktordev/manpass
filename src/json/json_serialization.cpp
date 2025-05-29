//
// Created by Andrzej Machnik on 29/05/2025.
//
#include "vault/Vault.h"
#include "vault/Folder.h"
#include "vault/Entry.h"
#include "vault/CredentialEntry.h"
#include "vault/NoteEntry.h"
#include "json/json.hpp"

using json = nlohmann::json;

namespace vault {

// serialize CredentialEntry
void to_json(json& j, const CredentialEntry& entry) {
    j = json{
        {"type", "CREDENTIAL"},
        {"username", entry.username},
        {"password", entry.password}
    };
}

// serializa NoteEntry
void to_json(json& j, const NoteEntry& entry) {
    j = json {
        {"type", "NOTE"},
        {"text", entry.noteText}
    };
}

// serialize Entry (generic)
void to_json(json& j, const Entry& entry) {
    switch (entry.getType()) {
        case EntryType::CREDENTIAL:
            to_json(j, dynamic_cast<const CredentialEntry&>(entry));
            break;
        case EntryType::NOTE:
            to_json(j, dynamic_cast<const NoteEntry&>(entry));
            break;
    }
}

// serialize Folder
void to_json(json& j, const Folder& folder) {
    j["name"] = folder.folderName;
    j["entries"] = json::array();
    for (const auto& [name, entry] : folder.entries) {
        j["entries"].push_back(*entry);
    }
}

// serialize Vault
void to_json(json& j, const Vault& vault) {
    j["name"] = vault.vaultName;
    j["folders"] = json::array();
    for (const auto& [name, folder] : vault.folders) {
        j["folders"].push_back(*folder);
    }
}


} // namespace vault