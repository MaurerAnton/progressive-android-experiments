#include "progressive/notification_summary.hpp"
#include <sstream>
#include <chrono>

namespace progressive {

static std::string extractField(const std::string& json, const std::string& key) {
    auto p = json.find("\"" + key + "\":\"");
    if (p == std::string::npos) return "";
    p += key.size() + 4;
    auto e = json.find('"', p);
    if (e == std::string::npos) return "";
    return json.substr(p, e - p);
}

std::vector<NotificationItem> parseNotificationItems(const std::string& json, int maxItems) {
    std::vector<NotificationItem> items;
    size_t pos = 0;
    while (pos < json.size() && (int)items.size() < maxItems) {
        auto evtPos = json.find("\"event_id\":\"", pos);
        if (evtPos == std::string::npos) break;
        
        NotificationItem item;
        item.eventId = extractField(json.substr(evtPos), "event_id");
        item.roomId = extractField(json.substr(evtPos), "room_id");
        
        auto bodyPos = json.find("\"body\":\"", evtPos);
        if (bodyPos != std::string::npos && bodyPos - evtPos < 500) {
            item.body = extractField(json.substr(evtPos), "body");
        }
        
        // Check highlight
        auto hlPos = json.find("\"highlight\":true", evtPos);
        if (hlPos != std::string::npos && hlPos - evtPos < 200) item.isHighlight = true;
        
        items.push_back(item);
        pos = evtPos + 20;
    }
    return items;
}

NotificationSummary buildNotificationSummary(const std::vector<NotificationItem>& items) {
    NotificationSummary s;
    s.items = items;
    for (const auto& item : items) {
        s.totalCount++;
        if (item.isHighlight) s.highlightCount++;
        if (item.isDirect) s.directCount++;
    }
    s.groupCount = s.totalCount;
    return s;
}

std::string formatNotificationBadge(const NotificationSummary& s) {
    if (s.totalCount == 0) return "";
    if (s.totalCount >= 1000) return "999+";
    return std::to_string(s.totalCount);
}

std::string formatNotificationGroupText(const std::string& roomName, int count, bool isDirect) {
    std::ostringstream os;
    if (count == 1) {
        return roomName + ": new message";
    }
    os << count << " new messages";
    if (!roomName.empty()) os << " in " << roomName;
    return os.str();
}

bool shouldSuppressNotification(const NotificationItem& item, int64_t nowMs) {
    if (item.body.empty()) return true;
    if (nowMs <= 0) {
        nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    }
    if (nowMs - item.timestampMs > 3600000) return true; // older than 1 hour
    return false;
}

} // namespace progressive
