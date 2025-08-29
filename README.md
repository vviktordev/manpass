# Manpass – Terminal-based password manager in C++

Manpass is a terminal-based, sessionless password manager written in C++ as a second-semester university project.
It provides encrypted storage for login credentials and notes inside structured vaults, all managed via a simple command-line interface (CLI).

>⚠️ This was a student project and is not maintained. Do not use it for securing real passwords.

## How to build (on Linux)

Clone the project and build it using `cmake`:

```bash
git clone https://github.com/vviktordev/manpass
cd manpass
mkdir build && cd build
cmake ..
make
```

Dependencies:

* g++
* cmake
* [Botan 3](https://botan.randombit.net/) (cryptography library)

## How to run

Manpass is a command-line tool. Run it with a command like:

```bash
./manpass <command>
```

### Examples

```bash
# create a vault named "safe"
./manpass add safe

# create a folder inside the vault
./manpass add safe/folder

# add credentials
./manpass add safe/folder/login -c

# add a note
./manpass add safe/folder/note -n

# show an entry
./manpass show safe/folder/note

# update credentials
./manpass update safe/folder/login

# delete folder or vault
./manpass delete safe/folder
./manpass delete safe
```

## About the implementation

Manpass is a simple terminal-based password manager.

* Supports full CRUD operations with 'add', 'show', 'update', and 'delete' commands.
* Every CRUD operation is supported by every entity (entry, folder, and vault)
* Data is encrypted and stored in JSON files ("vaults").
* Each vault is protected with a master password.
* Entries can be either credentials (username/password) or notes.
* Uses the Botan 3 library for encryption.

The program was tested on Linux and macOS.
