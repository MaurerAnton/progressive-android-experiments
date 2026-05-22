#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

struct NotificationGroup {
    std::string roomId;
    std::string roomName;
    std::string roomAvatar;
    std::string summaryText;     // "Alice: 5 new messages"
    int messageCount = 0;
    int highlightCount = 0;
    int64_t latestTimestampMs = 0;
    bool isDirect = false;
    bool isNoisy = false;
    std::vector<std::string> senderNames;  // unique senders
    std::vector<std::string> eventIds;
};

// Group notification events by room
std::vector<NotificationGroup> groupByRoom(
    const std::vector<std::string>& roomIds,
    const std::vector<std::string>& roomNames,
    const std::vector<std::string>& senderIds,
    const std::vector<std::string>& bodies,
    const std::vector<bool>& highlights,
    const std::vector<int64_t>& timestamps);

// Format group summary for notification tray
std::string formatGroupSummary(const NotificationGroup& group);

// Format Android notification style text (inbox style lines)
std::vector<std::string> formatNotificationLines(const NotificationGroup& group, int maxLines = 5);

// Build notification tag for Android grouping
std::string buildNotificationTag(const std::string& roomId, bool isDirect);

} // namespace progressive
