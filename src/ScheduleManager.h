#pragma once

#include <memory>
#include <vector>
#include <cstdint>

#include <Poco/Data/SQLite/Connector.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/Statement.h>

// schedule(id INTEGER PRIMARY KEY, teacher_name TEXT NOT NULL, subject_name TEXT, time_start INTEGER NOT NULL, time_end INTEGER NOT NULL, FOREIGN KEY(attendee_id) REFERENCES users(id));


struct Event
{
    int64_t event_id;
    uint64_t time_start;
    uint64_t time_end;
    std::string teacher_name;
    std::string subject_name;
    int64_t attendee_id;
};

class ScheduleManager
{
public:
    ScheduleManager(std::unique_ptr<Poco::Data::Session> session);

    bool createEvent(uint64_t time_start, uint64_t time_end, std::string& teacher_name, std::string& subject_name);
    bool modifyEvent(int64_t event_id, uint64_t time_start, uint64_t time_end, std::string& teacher_name, std::string& subject_name);
    bool removeEvent(int64_t event_id);

    bool placeBooking(int64_t event_id, int64_t attendee_id);
    bool freeBooking(int64_t event_id, int64_t attendee_id);

    std::vector<Event> getAllEvents();

private:
    std::unique_ptr<Poco::Data::Session> session;
};