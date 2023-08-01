#include "RequestHandlerFactory.h"
#include "SessionManager.h"

RequestHandlerFactory::RequestHandlerFactory(const std::string& db_path, const std::pair<std::string, std::string>& key_pair)
        : db_path(db_path), env("./templates/"), ec_key(key_pair.first, key_pair.second), session_manager(Poco::makeShared<Poco::Crypto::ECKey>(ec_key))
{
    Poco::Data::SQLite::Connector::registerConnector();
}

Poco::Net::HTTPRequestHandler* RequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest &req)
{
    // parse JWT and check if authorised, and if teacher role
    bool authorised = false;
    bool is_teacher = false;

    Poco::Net::NameValueCollection cookies;
    req.getCookies(cookies);

    if (cookies.has("session"))
    {
        std::string jwt = cookies.get("session");
        Poco::JWT::Token session_token;

        if (session_manager.hasValidSignature(jwt, session_token) && !session_manager.hasExpired(session_token))
        {
            authorised = true;

            // check if teacher role
            is_teacher = session_token.payload().has("is_teacher") && session_token.payload().get("is_teacher") > 0;
        }
    }


    // route requests
    nlohmann::json data;
    auto uri = req.getURI();
    auto method = req.getMethod();

    if (uri == "/" && method == "GET")
    {
        if (authorised)
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