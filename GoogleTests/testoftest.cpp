//
// Created by Andrzej Machnik on 28/05/2025.
//
#include "gtest/gtest.h"
#include "../include/vault/Vault.h"
#include "../include/vault/Folder.h"
#include "../include/vault/CredentialEntry.h"

using namespace vault;

TEST(SampleTest, AddsCorrectly) {
    EXPECT_EQ(1 + 1, 2);
}

TEST(VaultTest, AddAndRetrieveFolder) {
    vault::Vault vault("TestVault");
    auto folder = std::make_unique<vault::Folder>("Email");
    vault.addFolder(std::move(folder));

    EXPECT_NE(vault.getFolder("Email"), nullptr);
}

TEST(FolderTest, AddAndRetrieveEntry) {
    Folder f("Work");
    f.addEntry(std::make_unique<CredentialEntry>("user", "pass"), "GitHub");

    Entry* e = f.getEntry("GitHub");
    ASSERT_NE(e, nullptr);
    EXPECT_EQ(e->getType(), EntryType::CREDENTIAL);

    CredentialEntry* ce = dynamic_cast<CredentialEntry*>(e);
    ASSERT_NE(ce, nullptr);
    EXPECT_EQ(ce->getUsername(), "user");
}
