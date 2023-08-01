#include "ScheduleManager.h"

ScheduleManager::ScheduleManager(std::unique_ptr<Poco::Data::Session> session)
    : session(std::move(session))
{}

bool ScheduleManager::createEvent(uint64_t time_start, uint64_t time_end, std::string &teacher_name, std::string &subject_name)
{
    try {
        *session << "INSERT INTO schedule (time_start, time_end, teacher_name, subject_name) VALUES(?, ?, ?, ?)",
                Poco::Data::Keywords::use(time_start),
                Poco::Data::Keywords::use(time_end),
                Poco::Data::Keywords::use(teacher_name),
                Poco::Data::Keywords::use(subject_name),
                Poco::Data::Keywords::now;
        return true;
    } catch (Poco::Exception& ex) {
        // Log error or handle it
        return false;
    }
}

bool ScheduleManager::modifyEvent(int64_t event_id, uint64_t time_start, uint64_t time_end, std::string& teacher_name, std::string& subject_name)
{
    try
    {
        *session << "UPDATE schedule SET time_start = ?, time_end = ?, teacher_name = ?, subject_name = ? WHERE id = ?",
                Poco::Data::Keywords::use(time_start),
                Poco::Data::Keywords::use(time_end),
                Poco::Data::Keywords::use(teacher_name),
                Poco::Data::Keywords::use(subject_name),
                Poco::Data::Keywords::use(event_id),
                Poco::Data::Keywords::now;
        return true;
    }
    catch (Poco::Exception& ex) {
        // Log error or handle it
        return false;
    }
}

bool ScheduleManager::removeEvent(int64_t event_id) {
    try
    {
        *session << "DELETE FROM schedule WHERE id = ?",
                Poco::Data::Keywords::use(event_id),
                Poco::Data::Keywords::now;
        return true;
    }
    catch (Poco::Exception& ex)
    {
        // Log error or handle it
        return false;
    }
}

bool ScheduleManager::placeBooking(int64_t event_id, int64_t attendee_id)
{
    try
    {
        // check if attendee already exists
        int64_t current_attendee = 0;
        *session << "SELECT attendee_id FROM schedule WHERE id = ?",
            Poco::Data::Keywords::into(current_attendee),
            Poco::Data::Keywords::use(event_id),
            Poco::Data::Keywords::now;

        if (current_attendee == 0) {
            *session << "UPDATE schedule SET attendee_id = ? WHERE id = ?",
                    Poco::Data::Keywords::use(attendee_id),
                    Poco::Data::Keywords::use(event_id),
                    Poco::Data::Keywords::now;
            return true;
        }
        else
        {
            return false;
        }
    }
    catch (Poco::Exception& ex)
    {
        // Log error or handle it
        return false;
    }
}

bool ScheduleManager::freeBooking(int64_t event_id, int64_t attendee_id)
{
    try
    {
        // get attendee on event
        int64_t current_attendee = 0;
        *session << "SELECT attendee_id FROM schedule WHERE id = ?",
            Poco::Data::Keywords::into(current_attendee),
            Poco::Data::Keywords::use(event_id),
            Poco::Data::Keywords::now;

        // check user reserved in the first place
        if (current_attendee == attendee_id)
        {
            *session << "UPDATE schedule SET attendee_id = NULL WHERE id = ?",
                    Poco::Data::Keywords::use(event_id),
                    Poco::Data::Keywords::now;
            return true;
        }
        else
        {
            return false;
        }
    }
    catch (Poco::Exception& ex)
    {
        // Log error or handle it
        return false;
    }
}

std::vector<Event> ScheduleManager::getAllEvents()
{
    std::vector<Event> events;
    Event event;

    try
    {
        Poco::Data::Statement select_events(*session);

        select_events << "SELECT id, time_start, time_end, teacher_name, subject_name, attendee_id FROM schedule",
            Poco::Data::Keywords::into(event.event_id),
            Poco::Data::Keywords::into(event.time_start),
            Poco::Data::Keywords::into(event.time_end),
            Poco::Data::Keywords::into(event.teacher_name),
            Poco::Data::Keywords::into(event.subject_name),
            Poco::Data::Keywords::into(event.attendee_id),
            Poco::Data::Keywords::range(0, 1),
            Poco::Data::Keywords::now;

        while (!select_events.done())
        {
            select_events.execute();
            events.push_back(event);
        }
    }
    catch (Poco::Exception& e)
    {
        std::cerr << "Error getting events. Reason: " << e.what() << std::endl;
    }

    return events;
}

