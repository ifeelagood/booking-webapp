#pragma once

#include <cstdint>
#include <string>
#include <memory>

#include <Poco/Data/Session.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/SQLite/Connector.h>

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/NameValueCollection.h>

#ifdef WIN32
#include <Poco/Crypto/ECkey.h>
#else
#include <Poco/Crypto/ECKey.h>
#endif

#include <Poco/JSON/Object.h>

#include <inja/inja.hpp>
#include <nlohmann/json.hpp>

#include "SessionManager.h"

#include "handlers/DynamicTemplateHandler.h"
#include "handlers/StaticResourceHandler.h"
#include "handlers/NotFoundHandler.h"
#include "handlers/RegistrationHandler.h"
#include "handlers/LoginHandler.h"
#include "handlers/RedirectHandler.h"
#include "handlers/ScheduleHandler.h"
#include "handlers/ActionHandler.h"

class RequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
{
public:
    RequestHandlerFactory(const std::string& db_path, const std::pair<std::string, std::string>& key_pair);

    virtual Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest &req);

private:
    void querySession(const Poco::Net::HTTPServerRequest& req, bool& is_authorised, bool& is_teacher, int64_t& user_id);

    inja::Environment env;
    Poco::Crypto::ECKey ec_key;
    SessionManager session_manager;
    const std::string db_path;
};