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
        UPDATE_VAULT,
        UPDATE_FOLDER,
        UPDATE_CREDENTIAL,
        UPDATE_NOTE,
        DELETE,
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
    // TODO: Not sure how to implement that because (according to the current plan) the ShowCommand object has optional parameters for folder and entry. Will come back to this
    struct ShowCommandArgs : public CommandArgs {};

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

    struct UpdateCredentialCommandArgs : public CommandArgs {
        UpdateCredentialCommandArgs() : CommandArgs(CommandType::UPDATE_CREDENTIAL) {}
        std::string vault;
        std::string folder;
        std::string credential;
    };

    struct UpdateNoteCommandArgs : public CommandArgs {
        UpdateNoteCommandArgs() : CommandArgs(CommandType::UPDATE_NOTE) {}
        std::string vault;
        std::string folder;
        std::string note;
    };

    // DELETE COMMANDS
    // TODO: Same as ShowCommandArgs
    struct DeleteCommandArgs : public CommandArgs {};

    // GENERATE COMMAND
    // TODO: Still unsure about the format of this command...
    struct GenerateCommandArgs : public CommandArgs {};
}

#endif //COMMANDARGUMENTS_H