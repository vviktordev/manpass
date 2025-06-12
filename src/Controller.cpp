//
// Created by wiktor on 5/19/25.
//

#include "Controller.h"

// Helper function for casting CommandArgs to concrete types
template<typename Derived, typename Base>
std::unique_ptr<Derived> unique_cast(std::unique_ptr<Base>&& base) {
    if (auto* derived = dynamic_cast<Derived*>(base.get())) {
        base.release();
        return std::unique_ptr<Derived>(derived);
    }
    throw std::runtime_error("Could not cast args");
}

Controller::Controller(std::unique_ptr<CommandArgs> args, Storage& storageModule) : storage{storageModule} {
    switch (args->getType()) {
        case CommandType::ADD_VAULT: {
            auto addVaultArgs = unique_cast<AddVaultCommandArgs>(std::move(args));
            command = std::make_unique<AddVaultCommand>(addVaultArgs->vault, storage);
            break;
        }
        case CommandType::ADD_FOLDER: {
            auto addFolderArgs = unique_cast<AddFolderCommandArgs>(std::move(args));
            command = std::make_unique<AddFolderCommand>(addFolderArgs->vault, addFolderArgs->folder, storage);
            break;
        }
        case CommandType::ADD_CREDENTIAL: {
            auto addCredArgs = unique_cast<AddCredentialCommandArgs>(std::move(args));
            command = std::make_unique<AddCredentialCommand>(addCredArgs->vault, addCredArgs->folder, addCredArgs->credential, storage);
            break;
        }
        case CommandType::ADD_NOTE: {
            auto addNoteArgs = unique_cast<AddNoteCommandArgs>(std::move(args));
            command = std::make_unique<AddNoteCommand>(addNoteArgs->vault, addNoteArgs->folder, addNoteArgs->note, storage);
            break;
        }
        case CommandType::SHOW: {
            auto showVaultArgs = unique_cast<ShowCommandArgs>(std::move(args));
            command = std::make_unique<ShowCommand>(storage);
            break;
        }
        case CommandType::SHOW_VAULT: {
            auto showVaultArgs = unique_cast<ShowVaultCommandArgs>(std::move(args));
            command = std::make_unique<ShowVaultCommand>(showVaultArgs->vault, storage);
            break;
        }
        case CommandType::SHOW_FOLDER: {
            auto showFolderArgs = unique_cast<ShowFolderCommandArgs>(std::move(args));
            command = std::make_unique<ShowFolderCommand>(showFolderArgs->vault, showFolderArgs->folder, storage);
            break;
        }
        case CommandType::SHOW_ENTRY: {
            auto showEntryArgs = unique_cast<ShowEntryCommandArgs>(std::move(args));
            command = std::make_unique<ShowEntryCommand>(showEntryArgs->vault, showEntryArgs->folder, showEntryArgs->entry, storage);
            break;
        }
        case CommandType::UPDATE_VAULT: {
            auto updateVaultArgs = unique_cast<UpdateVaultCommandArgs>(std::move(args));
            command = std::make_unique<UpdateVaultCommand>(updateVaultArgs->vault, storage);
            break;
        }
        case CommandType::UPDATE_FOLDER: {
            auto updateFolderArgs = unique_cast<UpdateFolderCommandArgs>(std::move(args));
            command = std::make_unique<UpdateFolderCommand>(updateFolderArgs->vault, updateFolderArgs->folder, storage);
            break;
        }
        case CommandType::UPDATE_ENTRY: {
            auto updateEntryArgs = unique_cast<UpdateEntryCommandArgs>(std::move(args));
            command = std::make_unique<UpdateEntryCommand>(updateEntryArgs->vault, updateEntryArgs->folder, updateEntryArgs->entry, storage);
            break;
        }
        case CommandType::DELETE_VAULT: {
            auto deleteVaultArgs = unique_cast<DeleteVaultCommandArgs>(std::move(args));
            command = std::make_unique<DeleteVaultCommand>(deleteVaultArgs->vault, storage);
            break;
        }
        case CommandType::DELETE_FOLDER: {
            auto deleteFolderArgs = unique_cast<DeleteFolderCommandArgs>(std::move(args));
            command = std::make_unique<DeleteFolderCommand>(deleteFolderArgs->vault, deleteFolderArgs->folder, storage);
            break;
        }
        case CommandType::DELETE_ENTRY: {
            auto deleteEntryArgs = unique_cast<DeleteEntryCommandArgs>(std::move(args));
            command = std::make_unique<DeleteEntryCommand>(deleteEntryArgs->vault, deleteEntryArgs->folder, deleteEntryArgs->entry, storage);
            break;
        }
        default:
            throw std::runtime_error("Unknown or unsupported command type.");
    }
};

void Controller::run() {
    command->execute();
}
