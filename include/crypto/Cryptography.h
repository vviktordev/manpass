//
// Created by wiktor on 5/19/25.
//

#ifndef CRYPTOGRAPHY_H
#define CRYPTOGRAPHY_H

#include <string>
#include <sstream>

#include <stdexcept>
#include <vault/Vault.h>
#include <botan/auto_rng.h>
#include <botan/hex.h>
#include <botan/cipher_mode.h>
#include <botan/pwdhash.h>
#include <botan/exceptn.h>
#include <botan/version.h>
#include <botan/base64.h>

#include "json/json.hpp"
#include "EncryptedBlob.h"

using json = nlohmann::json;

namespace cryptography {

    class Cryptography {};

    EncryptedBlob encryptVault(const vault::Vault& vault, const std::string& masterPassword);
    vault::Vault decryptVault(const EncryptedBlob& encryptedVault, const std::string& masterPassword);

} // namespace cryptography

#endif //CRYPTOGRAPHY_H