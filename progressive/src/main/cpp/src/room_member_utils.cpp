#include "progressive/room_member_utils.hpp"
#include <sstream>

namespace progressive {

RoomMemberCount parseMemberCount(const std::string& json) {
    RoomMemberCount c;
    auto extract = [&](const std::string& key) -> int {
        auto p = json.find("\"" + key + "\":");
        if (p == std::string::npos) return 0;
        p += key.size() + 2;
        try { return std::stoi(json.substr(p)); } catch(...) { return 0; }
    };
    c.joined = extract("num_joined_members") + extract("joined_member_count");
    c.invited = extract("num_invited_members");
    return c;
}

std::string formatMemberCount(int count) {
    if (count == 1) return "1 member";
    return std::to_string(count) + " members";
}

std::string formatMemberCountWithOnline(int total, int online) {
    std::ostringstream os;
    os << formatMemberCount(total);
    if (online > 0) os << ", " << online << " online";
    return os.str();
}

std::string membershipToString(const std::string& m) {
    if (m == "join") return "joined";
    if (m == "invite") return "invited";
    if (m == "leave") return "left";
    if (m == "ban") return "banned";
    if (m == "knock") return "knocked";
    return m;
}

std::string formatMembershipEvent(const std::string& membership, const std::string& name,
                                    const std::string& reason) {
    std::ostringstream os;
    os << name << " " << membershipToString(membership) << " the room";
    if (!reason.empty()) os << ": " << reason;
    return os.str();
}

} // namespace progressive
