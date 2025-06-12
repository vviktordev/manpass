#include <iostream>
#include "Controller.h"

int main(int argc, char** argv) {
    std::optional<std::unique_ptr<CommandArgs>> parsedArgs;
    try {
        Parser parser(argc, argv);
        parsedArgs = parser.parse();
        if (!parsedArgs.has_value()) {
            std::cout << "Could not parse arguments" << std::endl;
            return -1;
        }
    } catch (std::exception& e) {
        std::cout << "Parsing error: " << e.what() << std::endl;
        return -1;
    }

    fs::path vaultsDir;
    try {
        vaultsDir = getDefaultVaultsDirectory();
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return -1;
    }

    try {
        Storage storage(vaultsDir);
        Controller controller(std::move(parsedArgs.value()), storage);

        controller.run();
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}