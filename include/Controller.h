//
// Created by wiktor on 5/19/25.
//

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include "Cryptography.h"
#include "Storage.h"

class Controller {
public:
    Controller(int parserResults, Cryptography* cryptoModule, Storage* storageModule);
    void run();

private:
    int parserResults;
    Cryptography* cryptoModule;
    Storage* storageModule;
    std::string masterPassword;
};


#endif //CONTROLLER_H