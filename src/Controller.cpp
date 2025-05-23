//
// Created by wiktor on 5/19/25.
//

#include "Controller.h"

#include "Cryptography.h"
#include "Storage.h"

Controller::Controller(int parserResults, Cryptography* cryptoModule, Storage* storageModule) :
    parserResults{parserResults},
    cryptoModule{cryptoModule},
    storageModule{storageModule}
{};