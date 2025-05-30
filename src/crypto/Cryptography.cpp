//
// Created by wiktor on 5/19/25.
//

#include "../../include/crypto/Cryptography.h"

namespace cryptography {

    EncryptedBlob encryptVault(const vault::Vault& vault, const std::string& masterPassword) {
        Botan::AutoSeeded_RNG rng;

        // 1. Generate a salt and iv
        auto salt = rng.random_vec(16);
        auto iv = rng.random_vec(12);  // recommended for AES-GCM

        // 2. Define algorithm and kdf names
        const std::string algorithm = "AES-256/GCM";
        const std::string kdf = "PBKDF2(SHA-256)";

        // 3. Derive key
        const size_t key_len = 32;
        auto pbkdf = Botan::PasswordHashFamily::create(kdf);
        if (!pbkdf)
            throw std::runtime_error("Provided KDF algorithm not available");

        auto pwdhash = pbkdf->from_iterations(100000);
        Botan::secure_vector<uint8_t> key(key_len);
        pwdhash->derive_key(key.data(), key_len, masterPassword.c_str(), masterPassword.size(), salt.data(), salt.size());

        // 4. Serialize a vault
        json j = vault;
        std::string plaintext = j.dump();

        // 5. AEAD encrypt
        auto enc = Botan::Cipher_Mode::create(algorithm, Botan::Cipher_Dir::ENCRYPTION);
        if (!enc)
            throw std::runtime_error("AEAD algorithm not available");

        enc->set_key(key);
        enc->start(iv);

        Botan::secure_vector<uint8_t> buffer(plaintext.begin(), plaintext.end());
        enc->finish(buffer);

        // Concatenate IV + ciphertext
        std::vector<uint8_t> combinedData;
        combinedData.insert(combinedData.end(), iv.begin(), iv.end());
        combinedData.insert(combinedData.end(), buffer.begin(), buffer.end());

        EncryptedBlob blob;
        blob.algorithm = algorithm;
        blob.kdf = kdf;
        blob.salt = Botan::base64_encode(salt);
        blob.data = Botan::base64_encode(combinedData);
        return blob;
    }

    vault::Vault decryptVault(const EncryptedBlob& encryptedVault, const std::string& masterPassword) {
        const std::string algorithm = encryptedVault.algorithm;
        const std::string kdf = encryptedVault.kdf;

        auto salt = Botan::base64_decode(encryptedVault.salt);
        auto combinedData = Botan::base64_decode(encryptedVault.data);

        if (combinedData.size() < 12)
            throw std::runtime_error("Invalid encrypted vault");


        // Extract IV and ciphertext
        Botan::secure_vector<uint8_t> iv(combinedData.begin(), combinedData.begin()+12);
        Botan::secure_vector<uint8_t> ciphertext(combinedData.begin()+12, combinedData.end());

        // Derive key
        const size_t key_len = 32;
        auto pbkdf = Botan::PasswordHashFamily::create(kdf);
        if (!pbkdf)
            throw std::runtime_error("Provided KDF algorithm not available");
        auto pwdhash = pbkdf->from_iterations(100000);
        Botan::secure_vector<uint8_t> key(key_len);
        pwdhash->derive_key(key.data(), key_len, masterPassword.c_str(), masterPassword.size(), salt.data(), salt.size());

        // Decrypt
        auto dec = Botan::Cipher_Mode::create(algorithm,Botan::Cipher_Dir::DECRYPTION);
        if (!dec)
            throw std::runtime_error("AEAD algorithm not available");

        dec->set_key(key);
        dec->start(iv);

        try {
            dec->finish(ciphertext);
        } catch (const Botan::Exception& e) {
            throw std::runtime_error("Decryption failed: " + std::string(e.what()));
        }

        std::string plaintext(ciphertext.begin(), ciphertext.end());
        json j = json::parse(plaintext);

        vault::Vault vault("");
        from_json(j, vault);
        return vault;
    }

} // namespace cryptography