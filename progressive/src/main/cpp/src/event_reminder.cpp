#include "progressive/event_reminder.hpp"
#include <sstream>
#include <chrono>
#include <cstdlib>
#include <algorithm>

namespace progressive {

int64_t ReminderManager::nowMs() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

ReminderManager::ReminderManager() {}

std::string ReminderManager::addReminder(const std::string& eventId, const std::string& roomId,
                                           const std::string& note, int64_t remindAtMs) {
    Reminder r;
    r.id = "rem_" + std::to_string(nowMs()) + "_" + std::to_string(rand());
    r.eventId = eventId;
    r.roomId = roomId;
    r.note = note;
    r.remindAtMs = remindAtMs;
    r.createdAtMs = nowMs();
    reminders_.push_back(r);
    return r.id;
}

void ReminderManager::cancel(const std::string& id) {
    reminders_.erase(std::remove_if(reminders_.begin(), reminders_.end(),
        [&](const Reminder& r) { return r.id == id; }), reminders_.end());
}

void ReminderManager::dismiss(const std::string& id) {
    for (auto& r : reminders_) {
        if (r.id == id) { r.dismissed = true; break; }
    }
}

std::vector<Reminder> ReminderManager::getPending() const {
    std::vector<Reminder> result;
    for (const auto& r : reminders_) {
        if (!r.fired && !r.dismissed) result.push_back(r);
    }
    return result;
}

std::vector<Reminder> ReminderManager::getDue(int64_t nowMs) {
    if (nowMs <= 0) nowMs = ReminderManager::nowMs();
    std::vector<Reminder> result;
    for (auto& r : reminders_) {
        if (!r.fired && !r.dismissed && r.remindAtMs <= nowMs) {
            r.fired = true;
            result.push_back(r);
        }
    }
    return result;
}

std::vector<Reminder> ReminderManager::getForEvent(const std::string& eventId) const {
    std::vector<Reminder> result;
    for (const auto& r : reminders_) {
        if (r.eventId == eventId && !r.dismissed) result.push_back(r);
    }
    return result;
}

std::vector<Reminder> ReminderManager::getForRoom(const std::string& roomId) const {
    std::vector<Reminder> result;
    for (const auto& r : reminders_) {
        if (r.roomId == roomId && !r.dismissed) result.push_back(r);
    }
    return result;
}

int ReminderManager::pendingCount() const {
    return (int)getPending().size();
}

std::string ReminderManager::toJson() const {
    std::ostringstream os;
    os << "[";
    for (size_t i = 0; i < reminders_.size(); i++) {
        if (i > 0) os << ",";
        const auto& r = reminders_[i];
        os << R"({"id":")" << r.id << R"(")";
        os << R"(,"eventId":")" << r.eventId << R"(")";
        os << R"(,"note":")" << r.note << R"(")";
        os << R"(,"remindAtMs":)" << r.remindAtMs;
        os << R"(,"fired":)" << (r.fired ? "true" : "false");
        os << "}";
    }
    os << "]";
    return os.str();
}

void ReminderManager::fromJson(const std::string& json) {
    reminders_.clear();
}

} // namespace progressive
