//
// Created by wiktor on 5/24/25.
//

#include "parser/Parser.h"

namespace parser {
    Parser::Parser(int argc_val, char** argv_val): argc(argc_val), argv(argv_val) {}

    std::optional<std::unique_ptr<CommandArgs>> Parser::parse() {
        // Destroy the current args object if present (in case the parse method is used multiple times). Better safe than sorry!
        this->returnCommandArgs.reset();

        CLI::App app;
        app.require_subcommand(1);

        std::string path;

        // Options for add
        CLI::App* addSubcommand = app.add_subcommand("add", "Add vault, folder, credential, or note");
        addSubcommand->add_option("path", path, "Path in vault/folder/entry format")->required();

        bool credentialFlag = false;
        addSubcommand->add_flag("-c,--credential", credentialFlag, "Entry type being added is credential");
        bool noteFlag = false;
        addSubcommand->add_flag("-n,--note", noteFlag, "Entry type being added is note");

        addSubcommand->callback([&]() {
            this->handleAddSubcommand(path, credentialFlag, noteFlag);
        });

        // Options for show
        CLI::App* showSubcommand = app.add_subcommand("show", "Show vault, folder, or entry");
        showSubcommand->add_option("path", path, "Path in vault/folder/entry format");
        showSubcommand->callback([&]() {
            this->handleShowSubcommand(path);
        });

        // Options for update
        CLI::App* updateSubcommand = app.add_subcommand("update", "Update vault, folder, or entry");
        updateSubcommand->add_option("path", path, "Path in vault/folder/entry format")->required();
        updateSubcommand->callback([&]() {
            this->handleUpdateSubcommand(path);
        });

        // Options for delete
        CLI::App* deleteSubcommand = app.add_subcommand("delete", "Delete vault, folder, or entry");
        deleteSubcommand->add_option("path", path, "Path in vault/folder/entry format")->required();
        deleteSubcommand->callback([&]() {
            this->handleDeleteSubcommand(path);
        });

        app.parse(argc, argv);

        return std::move(returnCommandArgs);
    }

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

        // Adding a vault
        if (!vault.empty() && folder.empty() && entry.empty()) {
            auto args = std::make_unique<AddVaultCommandArgs>();
            args->vault = vault;
            this->returnCommandArgs = std::move(args);
        }

        // Adding a folder
        if (!vault.empty() && !folder.empty() && entry.empty()) {
            auto args = std::make_unique<AddFolderCommandArgs>();
            args->vault = vault;
            args->folder = folder;
            this->returnCommandArgs = std::move(args);
        }

        // Adding an entry
        if (!vault.empty() && !folder.empty() && !entry.empty()) {
            if (!credentialFlag && !noteFlag)
                throw std::runtime_error("No entry type provided (use flags -c for credential or -n for note)");

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

    void Parser::handleShowSubcommand(const std::string &path) {
        std::string vault, folder, entry;
        this->parsePath(path, vault, folder, entry);

        // Showing all vaults
        if (vault.empty() && folder.empty() && entry.empty()) {
            auto args = std::make_unique<ShowCommandArgs>();
            this->returnCommandArgs = std::move(args);
        }

        // Showing vault
        if (!vault.empty() && folder.empty() && entry.empty()) {
            auto args = std::make_unique<ShowVaultCommandArgs>();
            args->vault = vault;
            this->returnCommandArgs = std::move(args);
        }

        // Showing folder
        if (!vault.empty() && !folder.empty() && entry.empty()) {
            auto args = std::make_unique<ShowFolderCommandArgs>();
            args->vault = vault;
            args->folder = folder;
            this->returnCommandArgs = std::move(args);
        }

        // Showing entry
        if (!vault.empty() && !folder.empty() && !entry.empty()) {
            auto args = std::make_unique<ShowEntryCommandArgs>();
            args->vault = vault;
            args->folder = folder;
            args->entry = entry;
            this->returnCommandArgs = std::move(args);
        }
    }

    void Parser::handleUpdateSubcommand(const std::string &path) {
        std::string vault, folder, entry;
        this->parsePath(path, vault, folder, entry);

        // Updating vault
        if (!vault.empty() && folder.empty() && entry.empty()) {
            auto args = std::make_unique<UpdateVaultCommandArgs>();
            args->vault = vault;
            this->returnCommandArgs = std::move(args);
        }

        // Updating folder
        if (!vault.empty() && !folder.empty() && entry.empty()) {
            auto args = std::make_unique<UpdateFolderCommandArgs>();
            args->vault = vault;
            args->folder = folder;
            this->returnCommandArgs = std::move(args);
        }

        // Updating entry
        if (!vault.empty() && !folder.empty() && !entry.empty()) {
            auto args = std::make_unique<UpdateEntryCommandArgs>();
            args->vault = vault;
            args->folder = folder;
            args->entry = entry;
            this->returnCommandArgs = std::move(args);
        }
    }

    void Parser::handleDeleteSubcommand(const std::string &path) {
        std::string vault, folder, entry;
        this->parsePath(path, vault, folder, entry);

        // Deleting vault
        if (!vault.empty() && folder.empty() && entry.empty()) {
            auto args = std::make_unique<DeleteVaultCommandArgs>();
            args->vault = vault;
            this->returnCommandArgs = std::move(args);
        }

        // Deleting folder
        if (!vault.empty() && !folder.empty() && entry.empty()) {
            auto args = std::make_unique<DeleteFolderCommandArgs>();
            args->vault = vault;
            args->folder = folder;
            this->returnCommandArgs = std::move(args);
        }

        // Deleting entry
        if (!vault.empty() && !folder.empty() && !entry.empty()) {
            auto args = std::make_unique<DeleteEntryCommandArgs>();
            args->vault = vault;
            args->folder = folder;
            args->entry = entry;
            this->returnCommandArgs = std::move(args);
        }
    }
}