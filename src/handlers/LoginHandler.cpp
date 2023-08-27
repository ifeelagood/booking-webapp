#include "LoginHandler.h"

#include <format>
#include <utility>

LoginHandler::LoginHandler(std::unique_ptr<Poco::Data::Session> session, std::shared_ptr<SessionManager> session_manager)
    : session(std::move(session)), session_manager(std::move(session_manager)), hasher()
{
}

void LoginHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    // get user & pass from form, otherwise throw 400
    Poco::Net::HTMLForm form;
    form.load(request, request.stream());

    std::string email, password;
    try
    {
        email = form.get("email");
        password = form.get("password");
    }
    catch (const Poco::NotFoundException &e) // missing form params
    {
        std::cerr << "Required form parameter missing. Reason: " << e.what() << std::endl;
        response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        response.send() << "There was a problem logging in. Please try again.";
        return;
    }

    // look for password hash in db.
    int user_id;
    int is_teacher;
    std::string pw_hash;
    Poco::Data::Statement hash_stmt(*session);
    hash_stmt << "SELECT id, is_teacher, hash FROM users WHERE email = ?",
        Poco::Data::Keywords::into(user_id),
        Poco::Data::Keywords::into(is_teacher),
        Poco::Data::Keywords::into(pw_hash),
        Poco::Data::Keywords::use(email),
        Poco::Data::Keywords::lowerLimit(1); // lowerLimit ensures at least a single valid row

    // if the above fails, it may be because they are not registered. We do NOT tell the user this.
    try
    {
        hash_stmt.execute();
    }
    catch (const std::exception &e) {
        std::cerr << "Error while querying user hash. Reason: " << e.what() << std::endl;
        response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        response.send() << "Invalid email and/or password.";
        return;
    }

    // now, validate password.
    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    response.setContentType("text/html");

    if (hasher.verifyPassword(password, pw_hash))
    {
        std::string jwt = session_manager->createSessionToken(user_id, is_teacher);

        response.set("Set-Cookie", std::format("session={}", jwt));
        response.redirect("/book");
    }
    else
    {
        response.send() << "Invalid email and/or password.";
    }
}