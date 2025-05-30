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
        std::string salt;   // base64 encoded
        std::string data;   // base64 encoded encrypted vault blob
    };

} // namespace cryptography


#endif //ENCRYPTEDBLOB_H
