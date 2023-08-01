#include "ScheduleHandler.h"

ScheduleHandler::ScheduleHandler(std::shared_ptr<inja::Environment> env, std::shared_ptr<ScheduleManager> schedule_manager, nlohmann::json &data)
        : env(std::move(env)), data(data), schedule_manager(schedule_manager)
{
}

void ScheduleHandler::handleRequest(Poco::Net::HTTPServerRequest &req, Poco::Net::HTTPServerResponse &resp)
{

    std::vector<Event> events = schedule_manager->getAllEvents();

    for (const auto& event : events)
    {
        std::cout << "attendee id: " << event.attendee_id << std::endl;
        nlohmann::json event_json;
        event_json["status"] = "NOT IMPLEMENTED";
        event_json["name"] = event.teacher_name;
        event_json["subject"] = event.subject_name;
        event_json["time"] = std::to_string(event.time_start) + ' ' + std::to_string(event.time_end);

        data["events"].push_back(event_json);
    }

    resp.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    resp.setContentType("text/html");

    std::ostream& out = resp.send();

    try
    {
        inja::Template template_obj = env->parse_template("schedule.html");
        env->render_to(out, template_obj, data);
    }
    catch (const std::exception& e) // TODO throw 500
    {
        std::cerr << "Error Rendering Template for URI " << req.getURI() << ": " << e.what() << std::endl;
        std::cerr << data << std::endl; // TODO removeme
    }

    out.flush();
}