#pragma once

#include <memory>

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include <inja/inja.hpp>
#include <nlohmann/json.hpp>



class DynamicTemplateHandler : public Poco::Net::HTTPRequestHandler
{
public:
    DynamicTemplateHandler(std::shared_ptr<inja::Environment> env, const std::string& template_path, const nlohmann::json& data);

    virtual void handleRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp);


private:
    std::shared_ptr<inja::Environment> env;
    std::string template_path;
    const nlohmann::json& data;
};