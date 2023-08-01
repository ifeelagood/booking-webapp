#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include <sodium.h>

class PasswordHasher
{
public:
    PasswordHasher();

    std::string hashPassword(const std::string& password);

    bool verifyPassword(const std::string& password, const std::string& hash);

private:
    static constexpr int SALT_LENGTH = crypto_pwhash_SALTBYTES;
    static constexpr int HASH_LENGTH = crypto_pwhash_STRBYTES;

};