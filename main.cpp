#include <iostream>
#include "parser/Parser.h"
#include "include/json/json.hpp"

using json = nlohmann::json;

int main(int argc, char** argv) {
    std::cout<<"Welcome to Manpass"<<std::endl;
    json j;
    j["project"] = "Manpass";
    j["year"] = 2025;
    std::cout << j.dump(4) << std::endl;
    return 0;
}