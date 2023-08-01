#include "RedirectHandler.h"

RedirectHandler::RedirectHandler(const std::string& endpoint)
        : endpoint(endpoint)
{
}

void RedirectHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    response.redirect(endpoint);
}