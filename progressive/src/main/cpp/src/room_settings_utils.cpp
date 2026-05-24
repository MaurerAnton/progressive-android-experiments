#include "progressive/room_settings_utils.hpp"
#include <sstream>

namespace progressive {

static std::string extractField(const std::string& json, const std::string& key) {
    auto p = json.find("\"" + key + "\":\"");
    if (p == std::string::npos) return "";
    p += key.size() + 4;
    auto e = json.find('"', p);
    if (e == std::string::npos) return "";
    return json.substr(p, e - p);
}
static int extractInt(const std::string& json, const std::string& key) {
    auto p = json.find("\"" + key + "\":");
    if (p == std::string::npos) return 0;
    p += key.size() + 2;
    try { return std::stoi(json.substr(p)); } catch(...) { return 0; }
}

RoomSettings parseRoomSettings(const std::string& json) {
    RoomSettings s;
    s.name = extractField(json, "name");
    s.topic = extractField(json, "topic");
    s.joinRule = extractField(json, "join_rule");
    s.historyVisibility = extractField(json, "history_visibility");
    s.guestAccess = extractField(json, "guest_access");
    s.isEncrypted = json.find("\"algorithm\":\"m.megolm") != std::string::npos;
    s.roomVersion = extractInt(json, "room_version");
    return s;
}

std::string buildRoomSettingsUpdate(const std::string& field, const std::string& value) {
    return R"({")" + field + R"(":")" + value + R"("})";
}

std::string formatRoomSettings(const RoomSettings& s) {
    std::ostringstream os;
    os << "Room: " << (s.name.empty() ? "(unnamed)" : s.name) << "\n";
    if (!s.topic.empty()) os << "Topic: " << s.topic << "\n";
    os << "Join: " << getJoinRuleLabel(s.joinRule) << "\n";
    os << "History: " << getHistoryVisibilityLabel(s.historyVisibility) << "\n";
    os << "Encrypted: " << (s.isEncrypted ? "Yes" : "No");
    return os.str();
}

std::string getJoinRuleLabel(const std::string& rule) {
    if (rule == "public") return "Anyone";
    if (rule == "invite") return "Invite only";
    if (rule == "knock") return "Knock";
    if (rule == "private") return "Private";
    if (rule == "restricted") return "Restricted";
    return rule;
}

std::string getHistoryVisibilityLabel(const std::string& vis) {
    if (vis == "world_readable") return "Anyone";
    if (vis == "shared") return "Members (since invited)";
    if (vis == "invited") return "Members (since joined)";
    if (vis == "joined") return "Members only";
    return vis;
}

std::string getGuestAccessLabel(const std::string& access) {
    return access == "can_join" ? "Allowed" : "Forbidden";
}

bool isValidJoinRule(const std::string& r) {
    return r == "public" || r == "invite" || r == "knock" || r == "private" || r == "restricted";
}
bool isValidHistoryVisibility(const std::string& v) {
    return v == "world_readable" || v == "shared" || v == "invited" || v == "joined";
}

} // namespace progressive
