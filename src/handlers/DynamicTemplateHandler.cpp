#include "DynamicTemplateHandler.h"

#include <utility>

#include <format>
#include <iostream>

DynamicTemplateHandler::DynamicTemplateHandler(std::shared_ptr<inja::Environment> env, const std::string& template_path, const nlohmann::json &data)
    : env(std::move(env)), data(data), template_path(template_path)
{
}

void DynamicTemplateHandler::handleRequest(Poco::Net::HTTPServerRequest &req, Poco::Net::HTTPServerResponse &resp)
{

    resp.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    resp.setContentType("text/html");

    std::ostream& out = resp.send();

    try
    {
        inja::Template template_obj = env->parse_template(template_path);
        env->render_to(out, template_obj, data);
    }
    catch (const std::exception& e) // TODO throw 500
    {
        std::cerr << "Error Rendering Template for URI " << req.getURI() << ": " << e.what() << std::endl;
        std::cerr << data << std::endl; // TODO removeme
    }

    out.flush();
}