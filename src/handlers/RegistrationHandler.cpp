#include <nlohmann/json.hpp>
#include "RegistrationHandler.h"

RegistrationHandler::RegistrationHandler(std::unique_ptr<Poco::Data::Session> session)
    : session(std::move(session)), hasher()
{
    // create user table if it doesn't exist in DB
}

void RegistrationHandler::handleRequest(Poco::Net::HTTPServerRequest &request, Poco::Net::HTTPServerResponse &response)
{
    Poco::Net::HTMLForm form;
    form.load(request, request.stream()); // as POST request, must use overload with body: https://docs.pocoproject.org/current/Poco.Net.HTMLForm.html

    // get form data and check for errors
    std::string first_name, last_name, email, password;
    try
    {
        first_name = form.get("first_name");
        last_name = form.get("last_name");
        email = form.get("email");
        password = form.get("password");
    }
    catch (const Poco::NotFoundException& e)
    {
        std::cerr << "Required form parameter missing. Reason: " << e.what() << std::endl;
        response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        response.send() << "Required form parameter missing";
        return;
    }

    // check if teacher (im going to programmers hell for this one)
    int is_teacher;
    try
    {
        is_teacher = std::stoi(form.get("is_teacher"));
    }
    catch (const Poco::NotFoundException& e)
    {
        e.what(); // woo yeah
        is_teacher = 0;
    }

    // try hash password, otherwise throw 500
    std::string pw_hash;
    try
    {
        pw_hash = hasher.hashPassword(password);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Could not hash password. Reason: " << e.what() << std::endl;
        response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        response.send() << "There was a problem with your registration. Please try again";
        return;
    }

    // check if user already exists in the database by email
    Poco::Data::Statement select(*session);
    int count = 0;
    select << "SELECT COUNT(*) FROM users WHERE email = ?",
        Poco::Data::Keywords::into(count),
        Poco::Data::Keywords::use(email);

    try
    {
        select.execute();
    }
    catch (const Poco::Data::DataException& e)
    {
        std::cerr << "Database query error: " << e.what() << std::endl;
        response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        response.send() << "There was a problem with your registration. Please try again";
        return;
    }

    if (count > 0)
    {
        response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        response.send() << "There was a problem with your registration. Please try again"; // TODO major vuln, fix error handling.
        return;
    }

    // insert user into database, otherwise throw 500
    Poco::Data::Statement insert_stmt(*session);
    insert_stmt << "INSERT INTO users (first_name, last_name, email, hash, is_teacher) VALUES (?, ?, ?, ?, ?)",
            Poco::Data::Keywords::use(first_name),
            Poco::Data::Keywords::use(last_name),
            Poco::Data::Keywords::use(email),
            Poco::Data::Keywords::use(pw_hash),
            Poco::Data::Keywords::use(is_teacher);

    try
    {
        insert_stmt.execute();
    }
    catch (const Poco::Data::DataException& e)
    {
        std::cerr << "Could not insert into db. Reason: " << e.what() << std::endl;
        response.setStatus(Poco::Net::HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
        response.send() << "There was a problem with your registration. Please try again";
        return;
    }

    // now send redirect...
    response.setStatus(Poco::Net::HTTPResponse::HTTP_FOUND);
    response.set("Location", request.get("Referer"));
    response.setContentType("text/html");

    std::ostream& out = response.send();
    out << "<html><body>Registration successful! Redirecting...</body></html>";
    out.flush();
}