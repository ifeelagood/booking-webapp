#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <fstream>

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/URI.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/SQLite/Connector.h>


#include "../PasswordHasher.h"
#include "../SessionManager.h"

class LoginHandler : public Poco::Net::HTTPRequestHandler
{
public:
    LoginHandler(std::unique_ptr<Poco::Data::Session> session, std::shared_ptr<SessionManager> session_manager);
    virtual void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response);
private:
    std::unique_ptr<Poco::Data::Session> session;
    std::shared_ptr<SessionManager> session_manager;
    PasswordHasher hasher;
};