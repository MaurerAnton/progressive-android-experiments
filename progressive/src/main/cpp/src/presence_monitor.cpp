#include "progressive/presence_monitor.hpp"
#include <sstream>
#include <chrono>
#include <algorithm>

namespace progressive {

PresenceMonitor::PresenceMonitor() {}

static std::string extractField(const std::string& json, const std::string& key) {
    auto p = json.find("\"" + key + "\":\"");
    if (p == std::string::npos) return "";
    p += key.size() + 4;
    auto e = json.find('"', p);
    if (e == std::string::npos) return "";
    return json.substr(p, e - p);
}

void PresenceMonitor::updateFromEvent(const std::string& userId, const std::string& json) {
    PresenceEntry e;
    e.userId = userId;
    e.presence = extractField(json, "presence");
    e.statusMsg = extractField(json, "status_msg");
    e.currentlyActive = json.find("\"currently_active\":true") != std::string::npos;
    
    auto ago = json.find("\"last_active_ago\":");
    if (ago != std::string::npos) {
        ago += 17;
        try { e.lastActiveAgo = std::stoll(json.substr(ago)); } catch(...) {}
    }
    
    e.lastChangedAt = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    entries_[userId] = e;
}

void PresenceMonitor::updateFromList(const std::string& json) {
    size_t pos = 0;
    while (true) {
        auto userPos = json.find("\"@", pos);
        if (userPos == std::string::npos) break;
        auto colon = json.find(':', userPos);
        if (colon == std::string::npos) break;
        std::string userId = json.substr(userPos + 1, colon - userPos - 1);
        updateFromEvent(userId, json.substr(userPos, 200));
        pos = colon + 1;
    }
}

PresenceEntry PresenceMonitor::getPresence(const std::string& userId) const {
    auto it = entries_.find(userId);
    return it != entries_.end() ? it->second : PresenceEntry{userId, "offline"};
}

std::vector<PresenceEntry> PresenceMonitor::getOnlineUsers() const {
    std::vector<PresenceEntry> result;
    for (const auto& [id, e] : entries_) {
        if (e.presence == "online") result.push_back(e);
    }
    return result;
}

std::vector<PresenceEntry> PresenceMonitor::getAllPresence() const {
    std::vector<PresenceEntry> result;
    for (const auto& [id, e] : entries_) result.push_back(e);
    return result;
}

bool PresenceMonitor::isOnline(const std::string& userId) const {
    auto it = entries_.find(userId);
    return it != entries_.end() && it->second.presence == "online";
}

size_t PresenceMonitor::onlineCount() const { return getOnlineUsers().size(); }

int64_t PresenceMonitor::lastActiveMs(const std::string& userId) const {
    auto it = entries_.find(userId);
    return it != entries_.end() ? it->second.lastActiveAgo : -1;
}

std::string PresenceMonitor::toJson() const {
    std::ostringstream os;
    os << "[";
    bool first = true;
    for (const auto& [id, e] : entries_) {
        if (!first) os << ","; first = false;
        os << R"({"userId":")" << id << R"(")";
        os << R"(,"presence":")" << e.presence << R"(")";
        os << R"(,"lastActiveAgo":)" << e.lastActiveAgo;
        os << R"(,"active":)" << (e.currentlyActive ? "true" : "false");
        os << "}";
    }
    os << "]";
    return os.str();
}

void PresenceMonitor::fromJson(const std::string& json) {
    entries_.clear();
    if (json.empty() || json == "[]") return;
}

void PresenceMonitor::clear() { entries_.clear(); }

} // namespace progressive
