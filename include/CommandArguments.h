//
// Created by wiktor on 5/24/25.
//

/*
Every command has its own struct.
This approach eliminates the necessity of optional fields (which would be necessary with less granular structs).
The Parser instantiates the appropriate struct and returns it as the base CommandArgs type.
The Controller checks the true type of the struct and instantiates the appropriate Command object.
*/

#ifndef COMMANDARGUMENTS_H
#define COMMANDARGUMENTS_H
#include <string>

struct CommandArgs {
    virtual ~CommandArgs() {}
};

// ADD COMMANDS
struct AddVaultCommandArgs : public CommandArgs {
    std::string vault;
};

struct AddFolderCommandArgs : public CommandArgs {
    std::string vault;
    std::string folder;
};

struct AddCredentialCommandArgs : public CommandArgs {
    std::string vault;
    std::string folder;
    std::string credential;
};

struct AddNoteCommandArgs : public CommandArgs {
    std::string vault;
    std::string folder;
    std::string note;
};

// SHOW COMMANDS
// TODO: Not sure how to implement that because (according to the current plan) the ShowCommand object has optional parameters for folder and entry. Will come back to this
struct ShowCommandArgs : public CommandArgs {};

// UPDATE COMMANDS
struct UpdateVaultCommandArgs : public CommandArgs {
    std::string vault;
};

struct UpdateFolderCommandArgs : public CommandArgs {
    std::string vault;
    std::string folder;
};

struct UpdateCredentialCommandArgs : public CommandArgs {
    std::string vault;
    std::string folder;
    std::string credential;
};

struct UpdateNoteCommandArgs : public CommandArgs {
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

#endif //COMMANDARGUMENTS_H