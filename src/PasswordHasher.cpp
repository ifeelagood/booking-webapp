#include "PasswordHasher.h"

PasswordHasher::PasswordHasher()
{
    if (sodium_init() == -1)
    {
        throw std::runtime_error("Libsodium initialisation failed.");
    }
}

std::string PasswordHasher::hashPassword(const std::string &password)
{
    std::vector<unsigned char> hash(HASH_LENGTH);


    int success = crypto_pwhash_str(
        reinterpret_cast<char *>(hash.data()),
        password.data(),
        password.size(),
        crypto_pwhash_OPSLIMIT_INTERACTIVE,
        crypto_pwhash_MEMLIMIT_INTERACTIVE
    );

    if (success != 0)
    {
        throw std::runtime_error("Password hashing failed.");
    }

    return std::string(hash.begin(), hash.end());
}

bool PasswordHasher::verifyPassword(const std::string &password, const std::string &hash)
{

    int result = crypto_pwhash_str_verify(
            hash.data(),
            password.data(),
            password.size()
    );

    return result == 0;
}