//
// Created by Andrzej Machnik on 28/05/2025.
//
#include "gtest/gtest.h"
#include "../include/vault/Vault.h"
#include "../include/vault/Folder.h"
#include "../include/vault/CredentialEntry.h"
#include "../include/vault/NoteEntry.h"
#include "../include/json/json.hpp"

using json = nlohmann::json;
using namespace vault;

// Dummy test case
TEST(SampleTest, AddsCorrectly) {
    EXPECT_EQ(1 + 1, 2);
}


// Test cases for different operations on the vault and it's structures
TEST(VaultTest, AddAndRetrieveFolder) {
    Vault vault("MyVault");
    std::unique_ptr<Folder> folder = std::make_unique<Folder>("Logins");
    vault.addFolder(std::move(folder));
    EXPECT_EQ(vault.getFolder("Logins").getName(), "Logins");
}

TEST(VaultTest, AddAndRetrieveFolderVerbose) {
    Vault vault("MyVault");
    auto folder = std::make_unique<Folder>("Logins");
    std::cout<<"vault name: "<<vault.getName()<< std::endl;
    std::cout << "Adding folder: " << folder->getName() << std::endl;
    vault.addFolder(std::move(folder));

    std::cout << "Vault folders: ";
    for (const auto& name : vault.getFolderNames()) {
        std::cout << name << " ";
    }
    std::cout << std::endl;
    if (vault.folderExists("Logins")) {
        std::cout << "Folder exists!\n";
        auto allfolders = vault.getAllFolders();
        for (const auto& folder : allfolders) {
            std::cout<<folder->getName()<<"\n";
        }
        //std::cout << vault.getFolder("Logins").getName() << "\n";
    } else {
        std::cout << "Folder not found!\n";
    }

    try {
        auto& f = vault.getFolder("Logins");
        std::cout << "Retrieved folder name: " << f.getName() << "\n";
        EXPECT_EQ(f.getName(), "Logins");
    } catch (const std::exception& e) {
        FAIL() << "Exception caught: " << e.what();
    }
}


TEST(VaultTest, DuplicateFolderThrows) {
    Vault vault("MyVault");
    vault.addFolder(std::make_unique<Folder>("Logins"));
    EXPECT_THROW(vault.addFolder(std::make_unique<Folder>("Logins")), std::runtime_error);
}

TEST(VaultTest, RetrieveNonexistentFolderThrows) {
    Vault vault("MyVault");
    EXPECT_THROW(vault.getFolder("NotThere"), std::out_of_range);
}

TEST(FolderTest, AddAndRetrieveEntry) {
    Folder folder("Logins");
    auto entry = std::make_unique<CredentialEntry>("user","pass");
    folder.addEntry(std::move(entry), "Google");
    EXPECT_EQ(folder.getEntry("Google").getType(), EntryType::CREDENTIAL);
}

TEST(FolderTest, DuplicateEntryThrows) {
    Folder folder("Logins");
    folder.addEntry(std::make_unique<NoteEntry>("some text"), "Google");
    EXPECT_THROW(folder.addEntry(std::make_unique<NoteEntry>("some other text"), "Google"), std::runtime_error);
}

TEST(FolderTest, RetrieveNonexistentEntryThrows) {
    Folder folder("Logins");
    EXPECT_THROW(folder.getEntry("Facebook"), std::out_of_range);
}

TEST(VaultTest, GetEntryThroughVault) {
    Vault vault("MainVault");
    auto folder = std::make_unique<Folder>("Accounts");
    folder->addEntry(std::make_unique<CredentialEntry>("john", "password"), "Email");
    vault.addFolder(std::move(folder));
    EXPECT_EQ(vault.getEntry("Accounts", "Email").getType(), EntryType::CREDENTIAL);
}

TEST(VaultTest, FolderAndEntryExistenceChecks) {
    Vault vault("SecureVault");
    EXPECT_FALSE(vault.folderExists("Empty"));
    vault.addFolder(std::make_unique<Folder>("Empty"));
    EXPECT_TRUE(vault.folderExists("Empty"));

    Folder& folder = vault.getFolder("Empty");
    EXPECT_FALSE(folder.entryExists("Key"));
    folder.addEntry(std::make_unique<NoteEntry>("12345"), "Key");
    EXPECT_TRUE(folder.entryExists("Key"));
}

TEST(VaultTest, MoveVaultMaintainsData) {
    Vault original("ToBeMoved");
    original.addFolder(std::make_unique<Folder>("Data"));
    Vault moved = std::move(original);
    EXPECT_TRUE(moved.folderExists("Data"));
}

// -----------------------------------------------------------------------------------------
// JSON serialization and deserialization tests

// ---------- CREDENTIAL ENTRY TESTS ----------

TEST(CredentialEntryTest, SerializeDeserialize) {
    CredentialEntry original("user123", "pass456");

    json j = original;
    EXPECT_EQ(j["type"], "CREDENTIAL");
    EXPECT_EQ(j["username"], "user123");
    EXPECT_EQ(j["password"], "pass456");

    std::unique_ptr<Entry> parsed = parseEntry(j);
    auto* cred = dynamic_cast<CredentialEntry*>(parsed.get());
    ASSERT_NE(cred, nullptr);

    EXPECT_EQ(cred->getUsername(), "user123");
    EXPECT_EQ(cred->getPassword(), "pass456");
}

TEST(CredentialEntryTest, HandlesEmptyFields) {
    CredentialEntry entry("", "");
    json j = entry;

    auto parsed = parseEntry(j);
    auto* cred = dynamic_cast<CredentialEntry*>(parsed.get());

    ASSERT_NE(cred, nullptr);
    EXPECT_EQ(cred->getUsername(), "");
    EXPECT_EQ(cred->getPassword(), "");
}

TEST(CredentialEntryTest, HandlesSpecialCharacters) {
    CredentialEntry entry("usérñámè", "päßwörd😊");
    json j = entry;

    auto parsed = parseEntry(j);
    auto* cred = dynamic_cast<CredentialEntry*>(parsed.get());

    ASSERT_NE(cred, nullptr);
    EXPECT_EQ(cred->getUsername(), "usérñámè");
    EXPECT_EQ(cred->getPassword(), "päßwörd😊");
}

TEST(CredentialEntryTest, MissingFieldsThrows) {
    json j = {
        {"type", "CREDENTIAL"},
        {"username", "admin"}
        // password is missing
    };

    EXPECT_THROW(parseEntry(j), std::invalid_argument);
}

TEST(CredentialEntryTest, ExtraFieldsIgnored) {
    json j = {
        {"type", "CREDENTIAL"},
        {"username", "user"},
        {"password", "1234"},
        {"extra", "ignored"}
    };

    auto parsed = parseEntry(j);
    auto* cred = dynamic_cast<CredentialEntry*>(parsed.get());

    ASSERT_NE(cred, nullptr);
    EXPECT_EQ(cred->getUsername(), "user");
    EXPECT_EQ(cred->getPassword(), "1234");
}

// ---------- NOTE ENTRY TESTS ----------

TEST(NoteEntryTest, SerializeDeserialize) {
    NoteEntry original("Secret note!");

    json j = original;
    EXPECT_EQ(j["type"], "NOTE");
    EXPECT_EQ(j["text"], "Secret note!");

    std::unique_ptr<Entry> parsed = parseEntry(j);
    auto* note = dynamic_cast<NoteEntry*>(parsed.get());
    ASSERT_NE(note, nullptr);

    EXPECT_EQ(note->getNoteText(), "Secret note!");
}

TEST(NoteEntryTest, HandlesEmptyNote) {
    NoteEntry entry("");
    json j = entry;

    auto parsed = parseEntry(j);
    auto* note = dynamic_cast<NoteEntry*>(parsed.get());

    ASSERT_NE(note, nullptr);
    EXPECT_EQ(note->getNoteText(), "");
}

TEST(NoteEntryTest, HandlesMultiLineNote) {
    std::string multiline = "Line 1\nLine 2\n\tIndented";
    NoteEntry entry(multiline);
    json j = entry;

    auto parsed = parseEntry(j);
    auto* note = dynamic_cast<NoteEntry*>(parsed.get());

    ASSERT_NE(note, nullptr);
    EXPECT_EQ(note->getNoteText(), multiline);
}

TEST(NoteEntryTest, InvalidFieldTypeThrows) {
    json j = {
        {"type", "NOTE"},
        {"note", 12345}  // Should be string
    };

    EXPECT_THROW(parseEntry(j), std::invalid_argument);
}

TEST(NoteEntryTest, ExtraFieldIgnored) {
    json j = {
        {"type", "NOTE"},
        {"text", "some note"},
        {"unexpected", "field"}
    };

    auto parsed = parseEntry(j);
    auto* note = dynamic_cast<NoteEntry*>(parsed.get());

    ASSERT_NE(note, nullptr);
    EXPECT_EQ(note->getNoteText(), "some note");
}

// ---------- UNKNOWN TYPE ----------

TEST(ParseEntryTest, UnknownTypeThrows) {
    json j = {
        {"type", "UNKNOWN"},
        {"data", "irrelevant"}
    };

    EXPECT_THROW(parseEntry(j), std::invalid_argument);
}

// ---------- FOLDER TESTS ----------

TEST(FolderTest, SerializeDeserializeSingleEntry) {
    Folder folder("My Folder");
    folder.addEntry(std::make_unique<CredentialEntry>("user", "pass"), "login1");

    json j = folder;
    EXPECT_EQ(j["name"], "My Folder");
    ASSERT_EQ(j["entries"].size(), 1);

    // Search for the entry with name "login1"
    const json* loginEntry = nullptr;
    for (const auto& entry : j["entries"]) {
        if (entry.contains("name") && entry["name"] == "login1") {
            loginEntry = &entry;
            break;
        }
    }

    ASSERT_NE(loginEntry, nullptr);
    EXPECT_EQ((*loginEntry)["type"], "CREDENTIAL");

    // Now deserialize and check correctness
    Folder deserialized("");
    from_json(j, deserialized);
    EXPECT_EQ(deserialized.getName(), "My Folder");

    ASSERT_TRUE(deserialized.entryExists("login1"));
    auto& entry = deserialized.getEntry("login1");
    auto* cred = dynamic_cast<CredentialEntry*>(&entry);
    ASSERT_NE(cred, nullptr);
    EXPECT_EQ(cred->getUsername(), "user");
    EXPECT_EQ(cred->getPassword(), "pass");
}


TEST(FolderTest, HandlesEmptyFolder) {
    Folder folder("Empty");
    json j = folder;

    EXPECT_EQ(j["name"], "Empty");
    EXPECT_TRUE(j["entries"].empty());

    Folder deserialized("");
    from_json(j, deserialized);
    EXPECT_EQ(deserialized.getName(), "Empty");
    EXPECT_TRUE(deserialized.getEntryNames().empty());
}

TEST(FolderTest, MixedEntriesSerializeDeserialize) {
    Folder folder("Mixed");
    folder.addEntry(std::make_unique<CredentialEntry>("alice", "a123"), "entry1");
    folder.addEntry(std::make_unique<NoteEntry>("Buy milk"), "entry2");

    json j = folder;
    ASSERT_EQ(j["entries"].size(), 2);

    Folder deserialized("");
    from_json(j, deserialized);

    ASSERT_TRUE(deserialized.entryExists("entry1"));
    ASSERT_TRUE(deserialized.entryExists("entry2"));

    auto* cred = dynamic_cast<CredentialEntry*>(&deserialized.getEntry("entry1"));
    ASSERT_NE(cred, nullptr);
    EXPECT_EQ(cred->getUsername(), "alice");

    auto* note = dynamic_cast<NoteEntry*>(&deserialized.getEntry("entry2"));
    ASSERT_NE(note, nullptr);
    EXPECT_EQ(note->getNoteText(), "Buy milk");
}

TEST(FolderDeserializationTest, MissingNameThrows) {
    json j = {
        {"entries", json::array()}
    };

    Folder folder("");
    EXPECT_THROW(from_json(j, folder), std::invalid_argument);
}

TEST(FolderDeserializationTest, NameWrongTypeThrows) {
    json j = {
        {"name", 123},  // name should be a string
        {"entries", json::array()}
    };

    Folder folder("");
    EXPECT_THROW(from_json(j, folder), std::invalid_argument);
}

TEST(FolderDeserializationTest, MissingEntriesThrows) {
    json j = {
        {"name", "Test Folder"}
    };

    Folder folder("");
    EXPECT_THROW(from_json(j, folder), std::invalid_argument);
}

TEST(FolderDeserializationTest, EntriesWrongTypeThrows) {
    json j = {
        {"name", "Test Folder"},
        {"entries", "not an array"}
    };

    Folder folder("");
    EXPECT_THROW(from_json(j, folder), std::invalid_argument);
}

TEST(FolderDeserializationTest, EntryMissingNameThrows) {
    json j = {
        {"name", "Folder"},
        {"entries", {{
            {"type", "NOTE"},
            {"text", "abc"}
        }}}
    };

    Folder folder("");
    EXPECT_THROW(from_json(j, folder), std::invalid_argument);
}

TEST(FolderDeserializationTest, ValidMinimalFolderDeserializes) {
    json j = {
        {"name", "My Folder"},
        {"entries", json::array()}
    };

    Folder folder("");
    EXPECT_NO_THROW(from_json(j, folder));
    EXPECT_EQ(folder.getName(), "My Folder");
    EXPECT_TRUE(folder.getEntryNames().empty());
}


// --------- VAULT TESTS -----------

TEST(VaultTest, SerializeDeserializeSingleFolder) {
    Vault vault("MainVault");

    auto folder = std::make_unique<Folder>("Logins");
    folder->addEntry(std::make_unique<CredentialEntry>("admin", "root"), "adminAccount");
    vault.addFolder(std::move(folder));

    json j = vault;
    EXPECT_EQ(j["name"], "MainVault");
    ASSERT_EQ(j["folders"].size(), 1);

    Vault deserialized("");
    from_json(j, deserialized);
    EXPECT_EQ(deserialized.getName(), "MainVault");

    ASSERT_TRUE(deserialized.folderExists("Logins"));
    const auto& folderRef = deserialized.getFolder("Logins");

    ASSERT_TRUE(folderRef.entryExists("adminAccount"));
    const auto& entry = folderRef.getEntry("adminAccount");
    auto* cred = dynamic_cast<const CredentialEntry*>(&entry);
    ASSERT_NE(cred, nullptr);
    EXPECT_EQ(cred->getUsername(), "admin");
    EXPECT_EQ(cred->getPassword(), "root");
}

TEST(VaultTest, HandlesEmptyVault) {
    Vault vault("EmptyVault");
    json j = vault;

    EXPECT_EQ(j["name"], "EmptyVault");
    EXPECT_TRUE(j["folders"].empty());

    Vault deserialized("");
    from_json(j, deserialized);
    EXPECT_EQ(deserialized.getName(), "EmptyVault");
    EXPECT_TRUE(deserialized.getFolderNames().empty());
}

TEST(VaultTest, MultipleFoldersAndEntriesSerializeDeserialize) {
    Vault vault("TestVault");

    auto accounts = std::make_unique<Folder>("Accounts");
    accounts->addEntry(std::make_unique<CredentialEntry>("alice", "a123"), "alice");
    accounts->addEntry(std::make_unique<CredentialEntry>("bob", "b456"), "bob");

    auto notes = std::make_unique<Folder>("Notes");
    notes->addEntry(std::make_unique<NoteEntry>("Call Alice"), "reminder");

    vault.addFolder(std::move(accounts));
    vault.addFolder(std::move(notes));

    json j = vault;

    Vault deserialized("");
    from_json(j, deserialized);

    ASSERT_TRUE(deserialized.folderExists("Accounts"));
    ASSERT_TRUE(deserialized.folderExists("Notes"));

    const auto& accFolder = deserialized.getFolder("Accounts");
    ASSERT_TRUE(accFolder.entryExists("alice"));
    ASSERT_TRUE(accFolder.entryExists("bob"));

    const auto& notesFolder = deserialized.getFolder("Notes");
    ASSERT_TRUE(notesFolder.entryExists("reminder"));

    const auto& note = notesFolder.getEntry("reminder");
    auto* noteEntry = dynamic_cast<const NoteEntry*>(&note);
    ASSERT_NE(noteEntry, nullptr);
    EXPECT_EQ(noteEntry->getNoteText(), "Call Alice");
}

TEST(VaultDeserializationTest, MissingNameThrows) {
    json j = {
        {"folders", json::array()}
    };

    Vault vault("placeholder");
    EXPECT_THROW(from_json(j, vault), std::invalid_argument);
}

TEST(VaultDeserializationTest, NameWrongTypeThrows) {
    json j = {
        {"name", true},
        {"folders", json::array()}
    };

    Vault vault("placeholder");
    EXPECT_THROW(from_json(j, vault), std::invalid_argument);
}

TEST(VaultDeserializationTest, MissingFoldersThrows) {
    json j = {
        {"name", "Vault"}
    };

    Vault vault("placeholder");
    EXPECT_THROW(from_json(j, vault), std::invalid_argument);
}

TEST(VaultDeserializationTest, FoldersWrongTypeThrows) {
    json j = {
        {"name", "Vault"},
        {"folders", "not an array"}
    };

    Vault vault("placeholder");
    EXPECT_THROW(from_json(j, vault), std::invalid_argument);
}

TEST(VaultDeserializationTest, ValidEmptyVaultDeserializes) {
    json j = {
        {"name", "My Vault"},
        {"folders", json::array()}
    };

    Vault vault("placeholder");
    EXPECT_NO_THROW(from_json(j, vault));
    EXPECT_EQ(vault.getName(), "My Vault");
    EXPECT_TRUE(vault.getFolderNames().empty());
}
