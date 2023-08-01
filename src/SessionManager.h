#pragma once

#include <cstdint>
#include <set>
#include <string>
#include <iostream>

#include <Poco/JWT/Token.h>
#include <Poco/JWT/Signer.h>
#include <Poco/JWT/JWTException.h>
#include <Poco/Crypto/ECKey.h>

#include <Poco/Data/Session.h>
#include <Poco/Data/Statement.h>

// see
// https://docs.pocoproject.org/current/Poco.JWT.Token.html & https://docs.pocoproject.org/current/Poco.JWT.Signer.html

class SessionManager
{
public:
    SessionManager(const Poco::SharedPtr<Poco::Crypto::ECKey>& ec_key);

    std::string createSessionToken(int64_t user_id, int is_teacher);
    bool hasValidSignature(const std::string& jwt, Poco::JWT::Token& token); // check authenticity of token
    static bool hasExpired(const Poco::JWT::Token& token); // check authenticity and expiration of token

private:
    Poco::JWT::Signer signer;
};