#include "progressive/notification_action_utils.hpp"
#include <sstream>

namespace progressive {

std::vector<NotificationAction> getDefaultActions() {
    return {
        {"reply", "Reply", "ic_notif_reply"},
        {"mark_read", "Mark read", "ic_notif_read"},
        {"dismiss", "Dismiss", "ic_notif_dismiss"}
    };
}
std::string buildActionIntent(const std::string& rid, const std::string& eid, const std::string& action) {
    std::ostringstream os;
    os << R"({"room_id":")" << rid << R"(","event_id":")" << eid << R"(","action":")" << action << R"("})";
    return os.str();
}
std::string parseQuickReplyResult(const std::string& json) {
    auto p = json.find("\"text\":\""); if (p == std::string::npos) return "";
    p += 8; auto e = json.find('"', p);
    return e != std::string::npos ? json.substr(p, e - p) : "";
}

} // namespace progressive
