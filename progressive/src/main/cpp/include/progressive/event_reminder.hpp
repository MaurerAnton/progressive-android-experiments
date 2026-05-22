#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

struct Reminder {
    std::string id;
    std::string eventId;        // event to remind about
    std::string roomId;
    std::string note;
    int64_t remindAtMs = 0;     // when to fire
    int64_t createdAtMs = 0;
    bool fired = false;
    bool dismissed = false;
};

class ReminderManager {
public:
    ReminderManager();

    std::string addReminder(const std::string& eventId, const std::string& roomId,
                             const std::string& note, int64_t remindAtMs);
    void cancel(const std::string& id);
    void dismiss(const std::string& id);

    std::vector<Reminder> getPending() const;         // not fired, not dismissed
    std::vector<Reminder> getDue(int64_t nowMs = 0);  // remindAtMs <= now
    std::vector<Reminder> getForEvent(const std::string& eventId) const;
    std::vector<Reminder> getForRoom(const std::string& roomId) const;

    std::string toJson() const;
    void fromJson(const std::string& json);
    int pendingCount() const;

private:
    std::vector<Reminder> reminders_;
    static int64_t nowMs();
};

} // namespace progressive
