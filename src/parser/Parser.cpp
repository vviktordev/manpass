//
// Created by wiktor on 5/24/25.
//

#include "parser/Parser.h"

// TODO: Error handling
namespace parser {
    Parser::Parser(int argc_val, char** argv_val): argc(argc_val), argv(argv_val) {}

    std::optional<std::unique_ptr<CommandArgs>> Parser::parse() {
        // Destroy the current args object if present (in case the parse method is used multiple times). Better safe than sorry!
        this->returnCommandArgs.reset();

        CLI::App app;
        app.require_subcommand(1);

        std::string path;

        CLI::App* addSubcommand = app.add_subcommand("add", "Add vault, folder, credential, or note");

        addSubcommand->add_option("path", path, "Path in vault/folder/entry format")->required();

        bool credentialFlag = false;
        addSubcommand->add_flag("-c,--credential", credentialFlag, "Entry type being added is credential");

        bool noteFlag = false;
        addSubcommand->add_flag("-n,--note", noteFlag, "Entry type being added is note");

        addSubcommand->callback([&]() {
            this->handleAddSubcommand(path, credentialFlag, noteFlag);
        });

        // TODO: Handle all the rest of the commands in similar fashion.
        // I'm leaving it for later because for now, adding entries is enough for testing, and as we code, we might change it. That way we have less changes to make

        app.parse(argc, argv);

        return std::move(returnCommandArgs);
    }

    // TODO: Consider what happens when splitSize > 3, and when path is /folder/entry, vault//entry, or vault/folder/. Should we allow empty segments?
    void Parser::parsePath(const std::string &path, std::string &vault, std::string &folder, std::string &entry) {
        std::stringstream stream(path);
        std::string segment;
        std::vector<std::string> split;

        while (std::getline(stream, segment, '/')) {
            split.push_back(segment);
        }
        int splitSize = split.size();

        if (splitSize >= 1) {
            vault = split.at(0);
        }
        if (splitSize >= 2) {
            folder = split.at(1);
        }
        if (splitSize >= 3) {
            entry = split.at(2);
        }
    }

    void Parser::handleAddSubcommand(const std::string &path, bool credentialFlag, bool noteFlag) {
        std::string vault, folder, entry;

        this->parsePath(path, vault, folder, entry);

        // Adding an entry
        if (!vault.empty() && !folder.empty() && !entry.empty()) {
            if (credentialFlag) {
                auto args = std::make_unique<AddCredentialCommandArgs>();
                args->vault = vault;
                args->folder = folder;
                args->credential = entry;
                this->returnCommandArgs = std::move(args);
            }

            if (noteFlag) {
                auto args = std::make_unique<AddNoteCommandArgs>();
                args->vault = vault;
                args->folder = folder;
                args->note = entry;
                this->returnCommandArgs = std::move(args);
            }
        }
    }

}