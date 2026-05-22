#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

struct NotificationItem {
    std::string eventId;
    std::string roomId;
    std::string roomName;
    std::string senderName;
    std::string body;
    bool isHighlight = false;
    bool isDirect = false;
    int64_t timestampMs = 0;
};

struct NotificationSummary {
    int totalCount = 0;
    int highlightCount = 0;
    int directCount = 0;
    int groupCount = 0;
    std::vector<NotificationItem> items;
};

// Parse notification items from sync response
std::vector<NotificationItem> parseNotificationItems(const std::string& syncJson, int maxItems = 10);

// Build notification summary from items
NotificationSummary buildNotificationSummary(const std::vector<NotificationItem>& items);

// Format notification summary for badge/tray
std::string formatNotificationBadge(const NotificationSummary& summary);

// Format Android notification group summary text
std::string formatNotificationGroupText(const std::string& roomName, int count, bool isDirect);

// Check if notification should be suppressed (noisy, empty, outdated)
bool shouldSuppressNotification(const NotificationItem& item, int64_t nowMs = 0);

} // namespace progressive
