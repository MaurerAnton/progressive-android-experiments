#include "progressive/room_info_utils.hpp"
#include <sstream>

namespace progressive {
static std::string extractField(const std::string& json, const std::string& key) {
    auto p = json.find("\"" + key + "\":\""); if (p == std::string::npos) return "";
    p += key.size() + 4; auto e = json.find('"', p);
    return e != std::string::npos ? json.substr(p, e - p) : "";
}
RoomInfo parseRoomInfo(const std::string& json, const std::string& roomId) {
    RoomInfo r; r.roomId = roomId;
    r.name = extractField(json, "name"); r.topic = extractField(json, "topic");
    r.avatarUrl = extractField(json, "avatar_url"); r.joinRule = extractField(json, "join_rule");
    auto mp = json.find("\"num_joined_members\":"); if (mp != std::string::npos) { mp += 21; try { r.memberCount = std::stoi(json.substr(mp)); } catch(...) {} }
    return r;
}
std::string formatRoomHeader(const RoomInfo& info) {
    std::ostringstream os; os << (info.name.empty() ? info.roomId : info.name);
    if (info.isEncrypted) os << " 🔒"; return os.str();
}
std::string parseRoomDisplayName(const std::string& name, const std::string& alias,
                                   const std::string& roomId, const std::vector<std::string>& names) {
    if (!name.empty()) return name;
    if (!alias.empty()) return alias;
    if (names.size() == 1) return names[0];
    if (names.size() == 2) return names[0] + " and " + names[1];
    return names.empty() ? roomId : "Group chat";
}
} // namespace progressive
