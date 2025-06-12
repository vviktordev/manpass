//
// Created by wiktor on 6/7/25.
//

#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include "Storage.h"

using namespace storage;

class Command {
public:
    virtual ~Command();
    virtual void execute() = 0;
};

class AddVaultCommand : public Command {
public:
    AddVaultCommand(std::string vaultName, Storage& storage);
    void execute() override;
private:
    const std::string vaultName;
    Storage& storage;
};

class AddFolderCommand : public Command {
public:
    AddFolderCommand(std::string vaultName, std::string folderName, Storage& storage);
    void execute() override;
private:
    std::string vaultName, folderName;
    Storage& storage;
};

class AddCredentialCommand : public Command {
public:
    AddCredentialCommand(std::string vaultName, std::string folderName, std::string credentialName, Storage& storage);
    void execute() override;
private:
    std::string vaultName, folderName, credentialName;
    Storage& storage;
};

class AddNoteCommand : public Command {
public:
    AddNoteCommand(std::string vaultName, std::string folderName, std::string noteName, Storage& storage);
    void execute() override;
private:
    std::string vaultName, folderName, noteName;
    Storage& storage;
};

// Show names of all vaults
class ShowCommand : public Command {
public:
    ShowCommand(Storage& storage);
    void execute() override;
private:
    Storage& storage;
};

class ShowVaultCommand : public Command {
public:
    ShowVaultCommand(std::string vaultName, Storage& storage);
    void execute() override;
private:
    std::string vaultName;
    Storage& storage;
};

class ShowFolderCommand : public Command {
public:
    ShowFolderCommand(std::string vaultName, std::string folderName, Storage& storage);
    void execute() override;
private:
    std::string vaultName, folderName;
    Storage& storage;
};

class ShowEntryCommand : public Command {
public:
    ShowEntryCommand(std::string vaultName, std::string folderName, std::string entryName, Storage& storage);
    void execute() override;
private:
    std::string vaultName, folderName, entryName;
    Storage& storage;
};

class UpdateVaultCommand : public Command {
public:
    UpdateVaultCommand(std::string vaultName, Storage& storage);
    void execute() override;
private:
    std::string vaultName;
    Storage& storage;
};

class UpdateFolderCommand : public Command {
public:
    UpdateFolderCommand(std::string vaultName, std::string folderName, Storage& storage);
    void execute() override;
private:
    std::string vaultName, folderName;
    Storage& storage;
};

class UpdateEntryCommand : public Command {
public:
    UpdateEntryCommand(std::string vaultName, std::string folderName, std::string entryName, Storage& storage);
    void execute() override;
private:
    std::string vaultName, folderName, entryName;
    Storage& storage;
};

class DeleteVaultCommand : public Command {
public:
    DeleteVaultCommand(std::string vaultName, Storage& storage);
    void execute() override;
private:
    std::string vaultName;
    Storage& storage;
};

class DeleteFolderCommand : public Command {
public:
    DeleteFolderCommand(std::string vaultName, std::string folderName, Storage& storage);
    void execute() override;
private:
    std::string vaultName, folderName;
    Storage& storage;
};

class DeleteEntryCommand : public Command {
public:
    DeleteEntryCommand(std::string vaultName, std::string folderName, std::string entryName, Storage& storage);
    void execute() override;
private:
    std::string vaultName, folderName, entryName;
    Storage& storage;
};

#endif //COMMAND_H