#include "RequestHandlerFactory.h"
#include "SessionManager.h"

RequestHandlerFactory::RequestHandlerFactory(const std::string& db_path, const std::pair<std::string, std::string>& key_pair)
        : db_path(db_path), env("./templates/"), ec_key(key_pair.first, key_pair.second), session_manager(Poco::makeShared<Poco::Crypto::ECKey>(ec_key))
{
    Poco::Data::SQLite::Connector::registerConnector();
}

void RequestHandlerFactory::querySession(const Poco::Net::HTTPServerRequest &req, bool &is_authorised, bool &is_teacher, int64_t &user_id)
{
    is_authorised = false;
    is_teacher = false;
    user_id = -1;

    Poco::Net::NameValueCollection cookies;
    req.getCookies(cookies);

    if (cookies.has("session"))
    {
        std::string jwt = cookies.get("session");
        Poco::JWT::Token session_token;

        if (session_manager.hasValidSignature(jwt, session_token) && !session_manager.hasExpired(session_token))
        {
            is_authorised = true;

            // check if teacher role
            is_teacher = session_token.payload().has("is_teacher") && session_token.payload().get("is_teacher") > 0;

            // get uid
            user_id = std::stoll(session_token.getSubject());
        }
    }
}

Poco::Net::HTTPRequestHandler* RequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest &req)
{
    // get contents of JWT
    int64_t user_id;
    bool is_authorised, is_teacher;

    querySession(req, is_authorised, is_teacher, user_id);

    // setup template data
    nlohmann::json data;
    data["is_teacher"] = is_teacher;
    data["is_authorised"] = is_authorised;
    // data["user_id"] = user_id;

    auto uri = req.getURI();
    auto method = req.getMethod();

    if (uri == "/" && method == "GET")
    {
        if (is_authorised)
        {
            return new RedirectHandler("/book");
        }
        else
        {
            // otherwise, login
            data["title"] = "Login & Registration";
            return new DynamicTemplateHandler(std::make_shared<inja::Environment>(env), "index.html", data);
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

    if (uri == "/book" && method == "GET")
    {
        data["title"] = "Schedule";
        return new ScheduleHandler(std::make_shared<inja::Environment>(env), std::make_shared<ScheduleManager>(std::make_unique<Poco::Data::Session>("SQLite", db_path)), data);
    }

    return new NotFoundHandler(std::make_shared<inja::Environment>(env), data);
}