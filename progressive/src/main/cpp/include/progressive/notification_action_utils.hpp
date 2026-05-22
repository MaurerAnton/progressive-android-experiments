#pragma once
#include <string>
#include <vector>

namespace progressive {

struct NotificationAction {
    std::string actionId;       // "reply", "mark_read", "dismiss"
    std::string label;          // "Reply", "Mark read"
    std::string icon;           // Android resource name
};

// Get default notification actions
std::vector<NotificationAction> getDefaultActions();

// Build notification action intent extra
std::string buildActionIntent(const std::string& roomId, const std::string& eventId,
                                const std::string& action);

// Format quick reply remote input result
std::string parseQuickReplyResult(const std::string& intentJson);

} // namespace progressive
