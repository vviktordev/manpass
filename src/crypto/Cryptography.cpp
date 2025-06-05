//
// Created by wiktor on 5/19/25.
//

#include "crypto/Cryptography.h"

namespace cryptography {

    std::string generateBase64Salt(size_t saltLengthBytes) {
        Botan::AutoSeeded_RNG rng;
        return Botan::base64_encode(rng.random_vec(saltLengthBytes));
    };

    EncryptedBlob encrypt(
        const std::string &plaintext,
        const std::string &masterPassword,
        const std::string &algo,
        const std::string &kdf,
        const std::string &base64Salt,
        int kdfIterations
    ) {
        // throw if passed algorithm isn't supported
        if (std::find(acceptedAlgorithms.begin(), acceptedAlgorithms.end(), algo) == acceptedAlgorithms.end()) {
            throw std::invalid_argument("Unsupported algorithm");
        }

        // throw if passed KDF isn't supported
        if (std::find(acceptedKDFs.begin(), acceptedKDFs.end(), kdf) == acceptedKDFs.end()) {
            throw std::invalid_argument("Unsupported KDF");
        }

        // Generate nonce
        Botan::AutoSeeded_RNG rng;
        Botan::secure_vector<uint8_t> nonce = rng.random_vec(12);

        // Decode salt
        Botan::secure_vector<uint8_t> salt = Botan::base64_decode(base64Salt);

        // Derive key
        std::unique_ptr<Botan::PasswordHashFamily> pbkdfFamily = Botan::PasswordHashFamily::create(kdf);
        if (!pbkdfFamily)
            throw std::runtime_error("Provided KDF algorithm not available");

        std::unique_ptr<Botan::PasswordHash> pbkdf = pbkdfFamily->from_params(kdfIterations);
        if (!pbkdf)
            throw std::runtime_error("Error instantiating pbkdf");

        const size_t keyLength = 32;
        Botan::secure_vector<uint8_t> key(keyLength);
        pbkdf->derive_key(key.data(), keyLength, masterPassword.c_str(), masterPassword.size(), salt.data(), salt.size());

        // Encrypt
        const auto enc = Botan::Cipher_Mode::create(algo, Botan::Cipher_Dir::Encryption);
        if (!enc)
            throw std::runtime_error("AEAD algorithm not available");

        enc->set_key(key);
        enc->start(nonce);

        Botan::secure_vector<uint8_t> buffer(plaintext.begin(), plaintext.end());
        enc->finish(buffer);

        EncryptedBlob blob;
        blob.algorithm = algo;
        blob.kdf = kdf;
        blob.kdfIterations = kdfIterations;
        blob.base64Salt = Botan::base64_encode(salt);
        blob.base64Nonce = Botan::base64_encode(nonce);
        blob.base64Ciphertext = Botan::base64_encode(buffer);

        return blob;
    }

    std::string decrypt(
        EncryptedBlob encrypted,
        const std::string& masterPassword
    ) {
        // Validate algorithm and KDF
        if (std::find(acceptedAlgorithms.begin(), acceptedAlgorithms.end(), encrypted.algorithm) == acceptedAlgorithms.end()) {
            throw std::invalid_argument("Unsupported algorithm");
        }

        if (std::find(acceptedKDFs.begin(), acceptedKDFs.end(), encrypted.kdf) == acceptedKDFs.end()) {
            throw std::invalid_argument("Unsupported KDF");
        }

        // Decode base64 data
        Botan::secure_vector<uint8_t> salt = Botan::base64_decode(encrypted.base64Salt);
        Botan::secure_vector<uint8_t> nonce = Botan::base64_decode(encrypted.base64Nonce);
        Botan::secure_vector<uint8_t> ciphertext = Botan::base64_decode(encrypted.base64Ciphertext);

        // Derive key using same parameters
        std::unique_ptr<Botan::PasswordHashFamily> pbkdfFamily = Botan::PasswordHashFamily::create(encrypted.kdf);
        if (!pbkdfFamily)
            throw std::runtime_error("Provided KDF algorithm not available");

        std::unique_ptr<Botan::PasswordHash> pbkdf = pbkdfFamily->from_params(encrypted.kdfIterations);
        if (!pbkdf)
            throw std::runtime_error("Error instantiating pbkdf");

        const size_t keyLength = 32;
        Botan::secure_vector<uint8_t> key(keyLength);
        pbkdf->derive_key(key.data(), keyLength, masterPassword.c_str(), masterPassword.size(), salt.data(), salt.size());

        // Decrypt
        const auto dec = Botan::Cipher_Mode::create(encrypted.algorithm, Botan::Cipher_Dir::Decryption);
        if (!dec)
            throw std::runtime_error("AEAD algorithm not available");

        dec->set_key(key);
        dec->start(nonce);

        // For GCM, ciphertext already contains the authentication tag
        Botan::secure_vector<uint8_t> buffer = ciphertext;
        dec->finish(buffer);

        // Convert decrypted data back to string
        return std::string(buffer.begin(), buffer.end());
    }

} // namespace cryptography