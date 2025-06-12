//
// Created by wiktor on 5/19/25.
//

/*
The Cryptography module provides the baseline needed for supporting multiple encryption algorithms and KDFs.
Right now it only supports AES-256/GCM and PBKDF-2(AES-256), thus many things (which shouldn't be) are currently hard-coded.
*/

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

namespace cryptography {
    const std::vector<std::string> acceptedAlgorithms({"AES-256/GCM"});
    const std::vector<std::string> acceptedKDFs({"PBKDF2(SHA-256)"});

    std::string generateBase64Salt(size_t saltLengthBytes = 16);

    EncryptedBlob encrypt(
        const std::string& plaintext,
        const Botan::secure_vector<char>& masterPassword,
        const std::string& algo,
        const std::string& kdf,
        const std::string& base64Salt,
        int kdfIterations = 500000
    );

    // Throws Botan::Invalid_Authentication_Tag on decryption failure
    std::string decrypt(
        EncryptedBlob encrypted,
        const Botan::secure_vector<char>& masterPassword
    );
} // namespace cryptography

#endif //CRYPTOGRAPHY_H