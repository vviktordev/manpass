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

// deserialize CredentialEntry
void from_json(const json& j, CredentialEntry& entry) {
    if (!j.contains("username") || !j["username"].is_string())
        throw std::invalid_argument("Username is missing or is not a string");
    if (!j.contains("password") || !j["password"].is_string())
        throw std::invalid_argument("Password is missing or is not a string");

    entry = CredentialEntry{j["username"], j["password"]};
}

// deserialize NoteEntry
void from_json(const json& j, NoteEntry& entry) {
    if (!j.contains("text") || !j["text"].is_string())
        throw std::invalid_argument("Note text is missing or is not a string");

    entry = NoteEntry{j["text"]};
}


// template to prevent code duplication in the following parseEntry function
template<typename T, typename... Args>
std::unique_ptr<Entry> make_and_load(const json& j, Args&&... args) {
    auto entry = std::make_unique<T>(std::forward<Args>(args)...);
    from_json(j, *entry);
    return std::unique_ptr<Entry>(std::move(entry));
}

// deserialize generic entry (needs to be parsed)
std::unique_ptr<Entry> parseEntry(const json& j) {
    std::string type = j["type"];

    if (type == "CREDENTIAL") {
        return make_and_load<CredentialEntry>(j, "", "");
    } else if (type == "NOTE") {
        return make_and_load<NoteEntry>(j, "");
    } else {
        throw std::invalid_argument("Unknown entry type");
    }
}

// deserialize Folder
void from_json(const json& j, Folder& folder) {
    if (!j.contains("name") || !j["name"].is_string())
        throw std::invalid_argument("Folder name is missing or is not a string");
    if (!j.contains("entries") || !j["entries"].is_array())
        throw std::invalid_argument("Folder entries is missing or is not an array");

    folder = Folder{j["name"]};

    for (const auto& entry : j["entries"]) {
        if (!entry.contains("name") || !entry["name"].is_string())
            throw std::invalid_argument("Entry name is missing or is not a string");

        folder.addEntry(parseEntry(entry), entry["name"]);
    }
}

// deserialize Vault
void from_json(const json& j, Vault& vault) {
    if (!j.contains("name") || !j["name"].is_string())
        throw std::invalid_argument("Vault name is missing or is not a string");
    if (!j.contains("folders") || !j["folders"].is_array())
        throw std::invalid_argument("Vault folders is missing or is not an array");

    vault = Vault(j["name"]);

    for (const auto& folder_json : j["folders"]) {
        if (!folder_json.contains("name") || !folder_json["name"].is_string())
            throw std::invalid_argument("Folder name is missing or is not a string");

        auto folder = std::make_unique<Folder>("");
        from_json(folder_json, *folder);
        vault.addFolder(std::move(folder));
    }
}



} // namespace vault