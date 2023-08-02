#include "ScheduleHandler.h"

ScheduleHandler::ScheduleHandler(std::shared_ptr<inja::Environment> env, std::shared_ptr<ScheduleManager> schedule_manager, int64_t user_id, nlohmann::json &data)
        : env(std::move(env)), data(data), schedule_manager(schedule_manager), user_id(user_id)
{
}

void ScheduleHandler::handleRequest(Poco::Net::HTTPServerRequest &req, Poco::Net::HTTPServerResponse &resp)
{

    std::vector<Event> events = schedule_manager->getAllEvents();

    for (const auto& event : events)
    {

        nlohmann::json event_json;

        if (user_id == event.attendee_id) event_json["status"] = "ATTENDING";
        else if (event.attendee_id == 0) event_json["status"] = "AVAILABLE";
        else event_json["status"] = "UNAVAILABLE";

        event_json["event_id"] = event.event_id;
        event_json["name"] = event.teacher_name;
        event_json["subject"] = event.subject_name;
        event_json["time_start"] = std::to_string(event.time_start);
        event_json["time_end"] = std::to_string(event.time_end);

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