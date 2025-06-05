//
// Created by Andrzej Machnik on 30/05/2025.
//

// include/crypto/EncryptedBlob.h
#ifndef ENCRYPTEDBLOB_H
#define ENCRYPTEDBLOB_H

#include <string>

namespace cryptography {

    struct EncryptedBlob {
        std::string algorithm;
        std::string kdf;
        int kdfIterations;
        std::string base64Salt;
        std::string base64Nonce;
        std::string base64Ciphertext;
    };

} // namespace cryptography


#endif //ENCRYPTEDBLOB_H
