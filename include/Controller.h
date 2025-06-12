//
// Created by wiktor on 5/19/25.
//

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include "parser/Parser.h"
#include "crypto/Cryptography.h"
#include "Storage.h"
#include "Command.h"

using namespace vault;
using namespace cryptography;
using namespace storage;
using namespace parser;

class Controller {
public:
    Controller(std::unique_ptr<CommandArgs> args, Storage& storageModule);
    void run();

private:
    Storage& storage;
    std::unique_ptr<Command> command;
};


#endif //CONTROLLER_H