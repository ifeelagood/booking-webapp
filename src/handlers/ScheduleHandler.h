#pragma once

#include <memory>

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include <inja/inja.hpp>
#include <nlohmann/json.hpp>

#include "../ScheduleManager.h"

class ScheduleHandler : public Poco::Net::HTTPRequestHandler
{
public:
    ScheduleHandler(std::shared_ptr<inja::Environment> env, std::shared_ptr<ScheduleManager> schedule_manager, nlohmann::json& data);

    virtual void handleRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp);


private:
    std::shared_ptr<inja::Environment> env;
    std::shared_ptr<ScheduleManager> schedule_manager;
    nlohmann::json data;
};