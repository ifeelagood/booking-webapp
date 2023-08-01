#pragma once

#include <vector>
#include <string>

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/Util/IniFileConfiguration.h>


#include "RequestHandlerFactory.h"

class ServerApplication : public Poco::Util::ServerApplication
{
protected:
    int main(const std::vector<std::string>&);

    void initialize(Poco::Util::Application& app) override;
};