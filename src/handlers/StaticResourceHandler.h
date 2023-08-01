#pragma once

#include <memory>
#include <string>
#include <fstream>

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>



class StaticResourceHandler : public Poco::Net::HTTPRequestHandler
{
public:
    StaticResourceHandler(const std::string& file_path, const std::string& content_type);

    virtual void handleRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp);

private:
    std::string file_path;
    std::string content_type;
};