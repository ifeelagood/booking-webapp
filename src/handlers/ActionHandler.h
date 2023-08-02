#pragma once

#include <cstdint>
#include <memory>

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include <Poco/Net/HTMLForm.h>

#include <Poco/JSON/JSON.h>

#include "../ScheduleManager.h"

class ActionHandler : public Poco::Net::HTTPRequestHandler
{
public:
    ActionHandler(std::shared_ptr<ScheduleManager> schedule_manager, int64_t user_id, bool is_authorised, bool is_teacher);

    virtual void handleRequest(Poco::Net::HTTPServerRequest& req, Poco::Net::HTTPServerResponse& resp);


private:
    std::shared_ptr<ScheduleManager> schedule_manager;
    int64_t user_id;
    bool is_teacher;
    bool is_authorised;
};