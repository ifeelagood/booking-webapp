cmake_minimum_required(VERSION 3.0)
project(cs_ia)

set(CMAKE_CXX_STANDARD 20)

set(SRC
        src/main.cpp
        src/RequestHandlerFactory.cpp
        src/ServerApplication.cpp
        src/PasswordHasher.cpp
        src/SessionManager.cpp
        src/ScheduleManager.cpp
        src/handlers/DynamicTemplateHandler.cpp
        src/handlers/NotFoundHandler.cpp
        src/handlers/StaticResourceHandler.cpp
        src/handlers/RegistrationHandler.cpp
        src/handlers/LoginHandler.cpp
        src/handlers/RedirectHandler.cpp

        src/handlers/ScheduleHandler.cpp src/handlers/ActionHandler.cpp)

set(INC
        src/RequestHandlerFactory.h
        src/ServerApplication.h
        src/PasswordHasher.h
        src/SessionManager.h
        src/ScheduleManager.h
        src/handlers/DynamicTemplateHandler.h
        src/handlers/NotFoundHandler.h
        src/handlers/StaticResourceHandler.h
        src/handlers/DynamicTemplateHandler.h
        src/handlers/RegistrationHandler.h
        src/handlers/LoginHandler.h
        src/handlers/RedirectHandler.h
        src/ScheduleManager.h)



add_executable(cs_ia ${SRC} ${INC})

include(CMakePackageConfigHelpers)
find_package(OpenSSL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(inja REQUIRED)

include_directories(${OPENSSL_INCLUDE_DIR})

if (WIN32)
    find_package(Poco REQUIRED COMPONENTS Net DataSQLite Util Crypto jwt)
    find_package(unofficial-sodium CONFIG REQUIRED)
    target_link_libraries(cs_ia PRIVATE Poco::Net Poco::DataSQLite Poco::Util Poco::Crypto Poco::JWT OpenSSL::Crypto ${OPENSSL_LIBRARIES} unofficial-sodium::sodium nlohmann_json::nlohmann_json pantor::inja)
elseif(UNIX AND NOT APPLE)
    find_package(Poco REQUIRED COMPONENTS Net DataSQLite Util Crypto JWT)
    find_library(SODIUM_LIB NAMES sodium)
    target_link_libraries(cs_ia PRIVATE Poco::Net Poco::DataSQLite Poco::Util Poco::JWT Poco::Crypto OpenSSL::Crypto ${OPENSSL_LIBRARIES} nlohmann_json::nlohmann_json pantor::inja ${SODIUM_LIB})
endif()



include(cmake/post_build.cmake)