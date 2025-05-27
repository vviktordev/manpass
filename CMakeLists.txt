cmake_minimum_required(VERSION 3.31)
project(manpass)

set(CMAKE_CXX_STANDARD 17)

add_executable(manpass main.cpp
        src/Controller.cpp
        src/Cryptography.cpp
        src/Storage.cpp
        src/Parser.cpp
        src/vault/Vault.cpp
        src/vault/Folder.cpp
        src/vault/CredentialEntry.cpp
        src/vault/NoteEntry.cpp
)

target_include_directories(manpass PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)