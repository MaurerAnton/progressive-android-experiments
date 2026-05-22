#include "progressive/room_avatar_utils.hpp"
#include <sstream>
#include <hash>

namespace progressive {

std::string generateRoomInitials(const std::string& name) {
    if (name.empty()) return "?";
    std::string initials;
    for (char c : name) {
        if (c != ' ' && c != '#' && c != '!' && initials.size() < 2)
            initials += (char)toupper(c);
    }
    return initials.empty() ? name.substr(0, 1) : initials;
}

std::string getRoomColor(const std::string& roomId) {
    size_t h = std::hash<std::string>{}(roomId);
    const char* colors[] = {"#E53935","#1E88E5","#43A047","#FB8C00",
                             "#8E24AA","#00ACC1","#3949AB","#C0CA33"};
    return colors[h % 8];
}

std::string formatRoomAvatarUrl(const std::string& mxc, const std::string& hs,
                                  const std::string& name, const std::string& roomId) {
    if (!mxc.empty()) {
        const std::string prefix = "mxc://";
        auto slash = mxc.find('/', prefix.size());
        return hs + "/_matrix/media/v3/thumbnail/" +
               mxc.substr(prefix.size(), slash - prefix.size()) + "/" +
               mxc.substr(slash + 1) + "?width=96&height=96&method=crop";
    }
    return ""; // caller renders initials with color
}

bool hasCustomRoomAvatar(const std::string& json) {
    return json.find("\"avatar_url\":\"mxc://") != std::string::npos;
}

std::string buildRoomAvatarChange(const std::string& url) {
    return R"({"avatar_url":")" + url + R"("})";
}

} // namespace progressive
