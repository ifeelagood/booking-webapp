#include "StaticResourceHandler.h"

StaticResourceHandler::StaticResourceHandler(const std::string& file_path, const std::string& content_type)
    : file_path(file_path), content_type(content_type)
{
}

void StaticResourceHandler::handleRequest(Poco::Net::HTTPServerRequest &req, Poco::Net::HTTPServerResponse &resp)
{
    resp.setStatus(Poco::Net::HTTPResponse::HTTP_OK);

    std::ifstream ifs = std::ifstream(file_path, std::ios_base::in);
    if (!ifs.is_open())
    {
        std::cerr << "Error opening file: " << file_path << std::endl;

        resp.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        return;
    }

    resp.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    resp.setContentType(content_type);

    std::ostream& out = resp.send();

    std::copy(
            std::istreambuf_iterator<char>(ifs),
            std::istreambuf_iterator<char>(),
            std::ostreambuf_iterator<char>(out)
    );

    ifs.close();
    out.flush();
}