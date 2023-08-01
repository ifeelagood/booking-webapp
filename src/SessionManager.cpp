#include "SessionManager.h"

SessionManager::SessionManager(const Poco::SharedPtr<Poco::Crypto::ECKey>& ec_key)
    : signer(ec_key)
{
    signer.addAlgorithm(Poco::JWT::Signer::ALGO_ES256);
}

std::string SessionManager::createSessionToken(unsigned int user_id, int is_teacher)
{
    Poco::Timestamp time_issued, time_expire;
    time_expire += Poco::Timespan(6 * 60 * 60, 0);  //  TODO make expiration set by config.ini; currently 6 hours


    // create JWT
    Poco::JWT::Token token;
    token.setType("JWT");
    token.setSubject(std::to_string(user_id));
    token.payload().set("is_teacher", is_teacher);
    token.setIssuedAt(time_issued);
    token.setExpiration(time_expire);

    std::string jwt;
    try
    {
         jwt = signer.sign(token, Poco::JWT::Signer::ALGO_ES256);
    }
    catch (const Poco::JWT::JWTException& e)
    {
        std::cerr << "Could not sign JWT for user id " << user_id << ". Reason: " << e.what() << std::endl;
        jwt = "ERROR";
    }

    // sign token
    return jwt;
}

bool SessionManager::hasValidSignature(const std::string &jwt, Poco::JWT::Token &token)
{
    bool success = false;
    try
    {
        signer.tryVerify(jwt, token);
        success = true;
    }
    catch (const Poco::JWT::SignatureVerificationException &e)
    {
        std::cerr << "Could not verify JWT. Reason: " << e.what() << std::endl;
    }
    return success;
}

bool SessionManager::hasExpired(const Poco::JWT::Token &token)
{
    return token.getExpiration() < Poco::Timestamp();
}