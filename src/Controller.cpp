//
// Created by wiktor on 5/19/25.
//

#include "Controller.h"

#include "../include/crypto/Cryptography.h"
#include "Storage.h"

Controller::Controller(int parserResults, Storage* storageModule) :
    parserResults{parserResults},
    storageModule{storageModule}
{};