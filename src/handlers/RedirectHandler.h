#pragma once

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

class RedirectHandler : public Poco::Net::HTTPRequestHandler
{
public:
    RedirectHandler(const std::string& endpoint);
    virtual void handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response);

private:
    std::string endpoint;
};