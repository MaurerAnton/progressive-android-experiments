#include "progressive/notification_grouping.hpp"
#include <sstream>
#include <algorithm>
#include <set>

namespace progressive {

std::vector<NotificationGroup> groupByRoom(
    const std::vector<std::string>& roomIds,
    const std::vector<std::string>& roomNames,
    const std::vector<std::string>& senderIds,
    const std::vector<std::string>& bodies,
    const std::vector<bool>& highlights,
    const std::vector<int64_t>& timestamps) {
    
    std::vector<NotificationGroup> groups;
    std::unordered_map<std::string, size_t> roomIndex;
    
    for (size_t i = 0; i < roomIds.size(); i++) {
        auto& rid = roomIds[i];
        auto it = roomIndex.find(rid);
        NotificationGroup* g;
        if (it == roomIndex.end()) {
            groups.push_back({});
            g = &groups.back();
            g->roomId = rid;
            g->roomName = i < roomNames.size() ? roomNames[i] : "";
            roomIndex[rid] = groups.size() - 1;
        } else {
            g = &groups[it->second];
        }
        
        g->messageCount++;
        if (i < highlights.size() && highlights[i]) g->highlightCount++;
        if (i < timestamps.size() && timestamps[i] > g->latestTimestampMs)
            g->latestTimestampMs = timestamps[i];
        if (i < senderIds.size())
            g->senderNames.push_back(senderIds[i]);
        if (i < bodies.size())
            g->eventIds.push_back(bodies[i]);
    }
    
    // Deduplicate sender names
    for (auto& g : groups) {
        std::sort(g.senderNames.begin(), g.senderNames.end());
        g.senderNames.erase(std::unique(g.senderNames.begin(), g.senderNames.end()),
                            g.senderNames.end());
    }
    
    return groups;
}

std::string formatGroupSummary(const NotificationGroup& g) {
    std::ostringstream os;
    if (g.messageCount == 1 && !g.senderNames.empty()) {
        os << g.senderNames[0] << ": new message";
    } else {
        os << g.messageCount << " new messages";
        if (!g.senderNames.empty()) {
            os << " from ";
            for (size_t i = 0; i < g.senderNames.size() && i < 2; i++) {
                if (i > 0) os << ", ";
                os << g.senderNames[i];
            }
            if (g.senderNames.size() > 2) os << " and others";
        }
    }
    return os.str();
}

std::vector<std::string> formatNotificationLines(const NotificationGroup& g, int maxLines) {
    std::vector<std::string> lines;
    for (int i = 0; i < (int)g.eventIds.size() && i < maxLines; i++) {
        std::string line = g.eventIds[i];
        if (line.size() > 80) line = line.substr(0, 77) + "...";
        if (i < (int)g.senderNames.size())
            line = g.senderNames[i] + ": " + line;
        lines.push_back(line);
    }
    if ((int)g.eventIds.size() > maxLines)
        lines.push_back("+ " + std::to_string(g.eventIds.size() - maxLines) + " more");
    return lines;
}

std::string buildNotificationTag(const std::string& roomId, bool direct) {
    return direct ? "dm_" + roomId : "room_" + roomId;
}

} // namespace progressive
