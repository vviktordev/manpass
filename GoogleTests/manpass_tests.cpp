//
// Created by Andrzej Machnik on 28/05/2025.
//
#include "gtest/gtest.h"
#include "../include/vault/Vault.h"
#include "../include/vault/Folder.h"
#include "../include/vault/CredentialEntry.h"
#include "../include/vault/NoteEntry.h"

using namespace vault;

TEST(SampleTest, AddsCorrectly) {
    EXPECT_EQ(1 + 1, 2);
}

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
