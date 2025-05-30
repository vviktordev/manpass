cmake_minimum_required(VERSION 3.31)
project(manpass)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Attempt to find Botan 3 using pkg-config
find_package(PkgConfig)

if (PkgConfig_FOUND)
    pkg_check_modules(BOTAN QUIET botan-3)
endif()

# Fallback if pkg-config fails (for Windows or manual installations)
if (NOT BOTAN_FOUND)
    find_path(BOTAN_INCLUDE_DIR botan/version.h
            PATHS /usr/include /usr/local/include /opt/homebrew/include
            PATH_SUFFIXES botan-3
    )

    find_library(BOTAN_LIBRARY NAMES botan-3
            PATHS /usr/lib /usr/local/lib /opt/homebrew/lib
    )

    if (BOTAN_INCLUDE_DIR AND BOTAN_LIBRARY)
        set(BOTAN_FOUND TRUE)
        set(BOTAN_INCLUDE_DIRS ${BOTAN_INCLUDE_DIR})
        set(BOTAN_LIBRARIES ${BOTAN_LIBRARY})
    endif()
endif()

if (NOT BOTAN_FOUND)
    message(FATAL_ERROR "Botan 3 library not found. Please install it using a package manager or build from source.")
endif()

include_directories(${BOTAN_INCLUDE_DIRS})
link_directories(${BOTAN_LIBRARY_DIRS})

add_executable(manpass
        main.cpp
        src/Controller.cpp
        src/crypto/Cryptography.cpp
        src/Storage.cpp
        src/parser/Parser.cpp
        src/vault/Vault.cpp
        src/vault/Folder.cpp
        src/vault/CredentialEntry.cpp
        src/vault/NoteEntry.cpp
        src/json/json_serialization.cpp
        src/json/json_deserialization.cpp
)

target_include_directories(manpass PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(manpass ${BOTAN_LIBRARIES})

add_library(manpass_core
        src/vault/Vault.cpp
        src/vault/Folder.cpp
        src/vault/CredentialEntry.cpp
        src/vault/NoteEntry.cpp
        src/json/json_serialization.cpp
        src/json/json_deserialization.cpp
        src/crypto/Cryptography.cpp
        src/Storage.cpp
)

target_include_directories(manpass_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(manpass_core ${BOTAN_LIBRARIES})

enable_testing()
add_subdirectory(GoogleTests)
