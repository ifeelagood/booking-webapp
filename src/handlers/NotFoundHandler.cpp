#include "NotFoundHandler.h"

NotFoundHandler::NotFoundHandler(std::shared_ptr<inja::Environment> env, const nlohmann::json& data)
    : env(std::move(env)), data(data)
{
}


void NotFoundHandler::handleRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp)
{
    resp.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    resp.setContentType("text/html");


    data["title"] = "404 - Not Found (or is it??)";
    std::string template_path = "404.html";


    std::ostream& out = resp.send();

    try
    {
        inja::Template template_obj = env->parse_template(template_path);
        env->render_to(out, template_obj, data);
    }
    catch (const std::exception& e) // TODO throw 500
    {
        std::cerr << "Error Rendering Template for URI " << req.getURI() << ": " << e.what() << std::endl;
    }

    out.flush();
}