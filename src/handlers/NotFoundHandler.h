#pragma once

#include <memory>
#include <string>
#include <utility>

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include <inja/inja.hpp>
#include <nlohmann/json.hpp>

#include "DynamicTemplateHandler.h"

class NotFoundHandler : public Poco::Net::HTTPRequestHandler
{
public:
    NotFoundHandler(std::shared_ptr<inja::Environment> env);

    virtual void handleRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp);


private:
    std::shared_ptr<inja::Environment> env;
};