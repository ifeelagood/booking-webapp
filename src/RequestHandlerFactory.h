#pragma once

#include <string>
#include <memory>

#include <Poco/Data/Session.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/SQLite/Connector.h>

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/NameValueCollection.h>
#include <Poco/Crypto/ECkey.h>

#include <inja/inja.hpp>
#include <nlohmann/json.hpp>

#include "SessionManager.h"

#include "handlers/DynamicTemplateHandler.h"
#include "handlers/StaticResourceHandler.h"
#include "handlers/NotFoundHandler.h"
#include "handlers/RegistrationHandler.h"
#include "handlers/LoginHandler.h"

class RequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
{
public:
    RequestHandlerFactory(const std::string& db_path, const std::pair<std::string, std::string>& key_pair);

    virtual Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest &req);

private:
    bool isAuthorised(const Poco::Net::HTTPServerRequest &req);

    inja::Environment env;
    Poco::Crypto::ECKey ec_key;
    SessionManager session_manager;
    const std::string db_path;
};