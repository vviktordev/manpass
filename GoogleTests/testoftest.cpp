//
// Created by Andrzej Machnik on 28/05/2025.
//
#include "gtest/gtest.h"
#include "../include/vault/Vault.h"
#include "../include/vault/Folder.h"
#include "../include/vault/CredentialEntry.h"

using namespace vault;

TEST(VaultTest, AddAndRetrieveFolder) {
    Vault v("MyVault");
    v.addFolder(std::make_unique<Folder>("Personal"));

    Folder* f = v.getFolder("Personal");
    ASSERT_NE(f, nullptr);
    EXPECT_EQ(f->getName(), "Personal");
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
