#include "ActionHandler.h"

ActionHandler::ActionHandler(std::shared_ptr<ScheduleManager> schedule_manager, int64_t user_id, bool is_authorised, bool is_teacher)
    : schedule_manager(schedule_manager), user_id(user_id), is_teacher(is_teacher), is_authorised(is_authorised)
{}

void ActionHandler::handleRequest(Poco::Net::HTTPServerRequest &req, Poco::Net::HTTPServerResponse &resp)
{
    Poco::Net::HTMLForm form;
    form.load(req, req.stream()); // as POST request, must use overload with body: https://docs.pocoproject.org/current/Poco.Net.HTMLForm.html

    bool success;
    std::string action;
    try
    {
        action = form.get("action");

        // check for teacher perms (wack)
        if ((action == "create_event" || action == "modify_event" || action == "remove_event") && !is_teacher)
        {
            std::cerr << "Not authorised to do action: " << action << std::endl;
            resp.setStatus(Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED);
            resp.send() << "unauthorized";
        }

        if (action == "book_event")
        {
            int64_t event_id = std::stoll(form.get("event_id"));
            success = schedule_manager->placeBooking(event_id, user_id);
        }
        else if (action == "free_event")
        {
            int64_t event_id = std::stoll(form.get("event_id"));
            success = schedule_manager->freeBooking(event_id, user_id);
        }
        else if (action == "create_event")
        {
            std::string teacher_name = form.get("teacher_name");
            std::string subject_name = form.get("subject_name");
            uint64_t time_start = std::stoull(form.get("time_start"));
            uint64_t time_end = std::stoull(form.get("time_end"));

            success = schedule_manager->createEvent(time_start, time_end, teacher_name, subject_name);
        }
        else if (action == "modify_event")
        {

            int64_t event_id = std::stoll(form.get("event_id"));
            std::string teacher_name = form.get("teacher_name");
            std::string subject_name = form.get("subject_name");
            uint64_t time_start = std::stoull(form.get("time_start"));
            uint64_t time_end = std::stoull(form.get("time_end"));

            success = schedule_manager->modifyEvent(event_id, time_start, time_end, teacher_name, subject_name);
        }
        else if (action == "remove_event")
        {
            int64_t event_id = std::stoll(form.get("event_id"));

            success = schedule_manager->removeEvent(event_id);
        }
        else
        {
            std::cerr << "Invalid action: " << action << std::endl;
            resp.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
            resp.send() << "Bad Request";
            return;
        }

        //
        std::string resp_str = success ? "success" : "failure";
        resp.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        resp.send() << resp_str;

    }
    catch (const Poco::NotFoundException& e)
    {
        std::cerr << "Required form parameter missing. Reason: " << e.what() << std::endl;
        resp.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
        resp.send() << "Bad Request";
        return;
    }
}