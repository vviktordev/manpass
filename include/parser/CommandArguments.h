//
// Created by wiktor on 5/24/25.
//

/*
Every command has its own struct.
This approach eliminates the necessity of optional fields (which would be necessary with less granular structs).
The Parser instantiates the appropriate concrete struct and returns it as std::unique_ptr<CommandArgs>.
Every concrete struct sets its CommandType value (enum) in the constructor.
The type is then accessible with the getType() method.
*/

#ifndef COMMANDARGUMENTS_H
#define COMMANDARGUMENTS_H

#include <string>

namespace parser {
    enum class CommandType {
        UNKNOWN,
        ADD_VAULT,
        ADD_FOLDER,
        ADD_CREDENTIAL,
        ADD_NOTE,
        SHOW,
        SHOW_VAULT,
        SHOW_FOLDER,
        SHOW_ENTRY,
        UPDATE_VAULT,
        UPDATE_FOLDER,
        UPDATE_ENTRY,
        DELETE_VAULT,
        DELETE_FOLDER,
        DELETE_ENTRY,
        GENERATE,
    };

    struct CommandArgs {
        CommandArgs(CommandType type_val = CommandType::UNKNOWN) : type(type_val) {}
        virtual ~CommandArgs() {}
        CommandType getType() const { return type; }
    private:
        CommandType type;
    };

    // ADD COMMANDS
    struct AddVaultCommandArgs : public CommandArgs {
        AddVaultCommandArgs() : CommandArgs(CommandType::ADD_VAULT) {}
        std::string vault;
    };

    struct AddFolderCommandArgs : public CommandArgs {
        AddFolderCommandArgs() : CommandArgs(CommandType::ADD_FOLDER) {}
        std::string vault;
        std::string folder;
    };

    struct AddCredentialCommandArgs : public CommandArgs {
        AddCredentialCommandArgs() : CommandArgs(CommandType::ADD_CREDENTIAL) {}
        std::string vault;
        std::string folder;
        std::string credential;
    };

    struct AddNoteCommandArgs : public CommandArgs {
        AddNoteCommandArgs() : CommandArgs(CommandType::ADD_NOTE) {}
        std::string vault;
        std::string folder;
        std::string note;
    };

    // SHOW COMMANDS
    struct ShowCommandArgs : public CommandArgs {
        ShowCommandArgs() : CommandArgs(CommandType::SHOW) {}
    };

    struct ShowVaultCommandArgs : public CommandArgs {
        ShowVaultCommandArgs() : CommandArgs(CommandType::SHOW_VAULT) {}
        std::string vault;
    };

    struct ShowFolderCommandArgs : public CommandArgs {
        ShowFolderCommandArgs() : CommandArgs(CommandType::SHOW_FOLDER) {}
        std::string vault, folder;
    };

    struct ShowEntryCommandArgs : public CommandArgs {
        ShowEntryCommandArgs() : CommandArgs(CommandType::SHOW_ENTRY) {}
        std::string vault, folder, entry;
    };

    // UPDATE COMMANDS
    struct UpdateVaultCommandArgs : public CommandArgs {
        UpdateVaultCommandArgs() : CommandArgs(CommandType::UPDATE_VAULT) {}
        std::string vault;
    };

    struct UpdateFolderCommandArgs : public CommandArgs {
        UpdateFolderCommandArgs() : CommandArgs(CommandType::UPDATE_FOLDER) {}
        std::string vault;
        std::string folder;
    };

    struct UpdateEntryCommandArgs : public CommandArgs {
        UpdateEntryCommandArgs() : CommandArgs(CommandType::UPDATE_ENTRY) {}
        std::string vault;
        std::string folder;
        std::string entry;
    };

    // DELETE COMMANDS
    struct DeleteVaultCommandArgs : public CommandArgs {
        DeleteVaultCommandArgs() : CommandArgs(CommandType::DELETE_VAULT) {}
        std::string vault;
    };

    struct DeleteFolderCommandArgs: public CommandArgs {
        DeleteFolderCommandArgs() : CommandArgs(CommandType::DELETE_FOLDER) {}
        std::string vault;
        std::string folder;
    };

    struct DeleteEntryCommandArgs : public CommandArgs {
        DeleteEntryCommandArgs() : CommandArgs(CommandType::DELETE_ENTRY) {}
        std::string vault;
        std::string folder;
        std::string entry;
    };
}

#endif //COMMANDARGUMENTS_H