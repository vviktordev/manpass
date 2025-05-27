//
// Created by Andrzej Machnik on 27/05/2025.
//

// Directory: inlclude/vault/Entry.h
#ifndef VAULT_ENTRY_H
#define VAULT_ENTRY_H

#include <string>

namespace vault {

class Entry {
public:
    virtual ~Entry() = default;
    virtual std::string getType() const = 0;
};

} // vault

#endif //VAULT_ENTRY_H
