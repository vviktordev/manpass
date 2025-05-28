#include <iostream>
#include "parser/Parser.h"

int main(int argc, char** argv) {
    parser::Parser parser(argc, argv);
    std::optional<std::unique_ptr<parser::CommandArgs>> args = parser.parse();
    std::cout << "abc" << std::endl;
    return 0;
}