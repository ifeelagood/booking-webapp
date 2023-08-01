#include "RequestHandlerFactory.h"

#include "SessionManager.h"

RequestHandlerFactory::RequestHandlerFactory(const std::string& db_path, const std::pair<std::string, std::string>& key_pair)
    : db_path(db_path), env("./templates/"), ec_key(key_pair.first, key_pair.second), session_manager(Poco::makeShared<Poco::Crypto::ECKey>(ec_key))
{
    Poco::Data::SQLite::Connector::registerConnector();
}

<<<<<<< HEAD
Poco::Net::HTTPRequestHandler* RequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest &req)
{
    bool is_authorised = false;

    Poco::JWT::Token session_token;
=======

bool RequestHandlerFactory::isAuthorised(const Poco::Net::HTTPServerRequest &req)
{
    // get cookies
>>>>>>> e591235 (added teacher role to: registration page, database and JWT payload.)
    Poco::Net::NameValueCollection cookies;
    req.getCookies(cookies);

    // check for session cookie
    if (cookies.has("session"))
    {
        std::string jwt = cookies.get("session");
<<<<<<< HEAD
        bool verified = session_manager.hasValidSignature(jwt, session_token);
        bool expired = SessionManager::hasExpired(session_token);


        std::cout << "verified: " << verified << std::endl;
        std::cout << "expired: " << expired << std::endl;

        if (verified && !expired)
        {
            is_authorised = true;
        }
    }

    std::cout << "Is Authorised: " << is_authorised << std::endl;

=======
        Poco::JWT::Token session_token;

        if (session_manager.hasValidSignature(jwt, session_token) && !SessionManager::hasExpired(session_token)) // return true if sig valid and has not expired. short circut saves us here
        {
            return true;
        }
    }

    return false;
}

Poco::Net::HTTPRequestHandler* RequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest &req)
{
>>>>>>> e591235 (added teacher role to: registration page, database and JWT payload.)
    nlohmann::json data;
    auto uri = req.getURI();
    auto method = req.getMethod();

<<<<<<< HEAD
=======

>>>>>>> e591235 (added teacher role to: registration page, database and JWT payload.)
    if (uri == "/" && method == "GET")
    {
        if (is_authorised)
        {
            return new NotFoundHandler(std::make_shared<inja::Environment>(env));
        }
        else
        {
            // otherwise, login
            data["title"] = "Login & Registration";
            std::string template_path = "index.html";
            return new DynamicTemplateHandler(std::make_shared<inja::Environment>(env), template_path, data);
        }
    }
    if (uri == "/css/styles.css" && method == "GET")
    {
        return new StaticResourceHandler("css/styles.css", "text/css");
    }
    if (uri == "/register" && method == "POST")
    {
        return new RegistrationHandler(std::make_unique<Poco::Data::Session>("SQLite", db_path));
    }
    if (uri == "/login" && method == "POST")
    {
        return new LoginHandler(std::make_unique<Poco::Data::Session>("SQLite", db_path), std::make_shared<SessionManager>(Poco::makeShared<Poco::Crypto::ECKey>(ec_key)));
    }



    return new NotFoundHandler(std::make_shared<inja::Environment>(env));

}