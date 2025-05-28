//
// Created by wiktor on 5/24/25.
//

#ifndef PARSER_H
#define PARSER_H

#include <optional>

#include "CommandArguments.h"
#include "CLI11.hpp"

namespace parser {
    class Parser {
    public:
        Parser(int argc, char** argv);
        // The pointer is wrapped in std::optional because the args might not map to any CommandArgs
        std::optional<std::unique_ptr<CommandArgs>> parse();

    private:
        int argc;
        char** argv;
        // Holder property used for holding the CommandArgs object to be returned by parse()
        std::optional<std::unique_ptr<CommandArgs>> returnCommandArgs;

        void parsePath(const std::string& path, std::string &vault, std::string &folder, std::string &entry);
        // Methods used as callbacks in parse(). They set the returnCommandArgs property
        void handleAddSubcommand(const std::string& path, bool credentialFlag, bool noteFlag);
        void handleShowSubcommand(const std::string& path);
        void handleUpdateSubcommand(const std::string& path);
        void handleDeleteSubcommand(const std::string& path);
        void handleGenerateSubcommand(const std::string& path);
    };
}

#endif //PARSER_H