#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace progressive {

struct PresenceEntry {
    std::string userId;
    std::string presence;       // "online", "offline", "unavailable"
    std::string statusMsg;
    int64_t lastActiveAgo = 0;
    int64_t lastChangedAt = 0;
    bool currentlyActive = false;
};

class PresenceMonitor {
public:
    PresenceMonitor();

    void updateFromEvent(const std::string& userId, const std::string& eventJson);
    void updateFromList(const std::string& json); // from /sync presence
    PresenceEntry getPresence(const std::string& userId) const;
    std::vector<PresenceEntry> getOnlineUsers() const;
    std::vector<PresenceEntry> getAllPresence() const;

    bool isOnline(const std::string& userId) const;
    size_t onlineCount() const;
    int64_t lastActiveMs(const std::string& userId) const;

    std::string toJson() const;
    void fromJson(const std::string& json);
    void clear();

private:
    std::unordered_map<std::string, PresenceEntry> entries_;
};

} // namespace progressive
