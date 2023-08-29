#include "ServerApplication.h"

#include <iostream>
#include <Poco/Data/Session.h>
#include <Poco/Data/SQLite/Connector.h>
#include <Poco/Crypto/OpenSSLInitializer.h>
#ifdef WIN32
#include <openssl/applink.c> // <--- THIS FUCKER. RIGHT HERE. NEEDED TO STOP "OPENSSL_Uplink(00007FFF6DF94068,08): no OPENSSL_Applink"
#endif

void ServerApplication::initialize(Poco::Util::Application &app)
{
    // get config
    Poco::AutoPtr<Poco::Util::IniFileConfiguration> ini_config(new Poco::Util::IniFileConfiguration("config.ini"));
    config().add(ini_config);

    // init openssl
    Poco::Crypto::OpenSSLInitializer::initialize();

    // create db tables
    Poco::Data::SQLite::Connector::registerConnector();
    Poco::Data::Session session("SQLite", config().getString("database.path"));

    Poco::Data::Statement create_user_table(session);
    create_user_table << "CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, first_name TEXT, last_name TEXT, email TEXT, hash TEXT, is_teacher INTEGER);";
    create_user_table.execute();

    Poco::Data::Statement create_schedule_table(session);
    create_schedule_table << "CREATE TABLE IF NOT EXISTS schedule(id INTEGER PRIMARY KEY, teacher_name TEXT NOT NULL, subject_name TEXT, time_start INTEGER NOT NULL, time_end INTEGER NOT NULL, attendee_id INTEGER, FOREIGN KEY(attendee_id) REFERENCES users(id) );";
    create_schedule_table.execute();

    session.close();

    Poco::Util::ServerApplication::initialize(app);
}

int ServerApplication::main(const std::vector<std::string>&)
{
    // https://docs.pocoproject.org/current/Poco.Crypto.ECKey.html


    // get key path
    std::pair<std::string, std::string> key_pair = {config().getString("crypto.public_key"), config().getString("crypto.private_key")};

    // create and start server
    Poco::Net::HTTPServer server(
            new RequestHandlerFactory(config().getString("database.path"), key_pair),
            Poco::Net::ServerSocket(config().getInt("server.port")),
            new Poco::Net::HTTPServerParams
    );

    server.start();
    std::cout << "Server started" << std::endl;

    waitForTerminationRequest();

    std::cout << "Server stopping..." << std::endl;

    return Poco::Util::Application::EXIT_OK;
}