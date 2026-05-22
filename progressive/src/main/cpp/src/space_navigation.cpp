#include "progressive/space_navigation.hpp"
#include <sstream>
#include <algorithm>

namespace progressive {

static std::string extractField(const std::string& json, const std::string& key) {
    auto p = json.find("\"" + key + "\":\"");
    if (p == std::string::npos) return "";
    p += key.size() + 4;
    auto e = json.find('"', p);
    return e != std::string::npos ? json.substr(p, e - p) : "";
}

SpaceNode parseSpaceChild(const std::string& stateKey, const std::string& json) {
    SpaceNode node;
    node.roomId = stateKey;
    node.isSuggested = json.find("\"suggested\":false") == std::string::npos;
    node.name = extractField(json, "name");
    return node;
}

std::string parseSpaceParent(const std::string& json) {
    return extractField(json, "room_id");
}

SpaceHierarchy buildSpaceHierarchy(const std::string& spaceId,
                                     const std::vector<std::string>& keys,
                                     const std::vector<std::string>& contents,
                                     const std::vector<std::string>& names) {
    SpaceHierarchy h;
    h.rootId = spaceId;
    
    for (size_t i = 0; i < keys.size(); i++) {
        SpaceNode node = parseSpaceChild(keys[i], contents[i]);
        if (i < names.size()) node.name = names[i];
        h.children.push_back(keys[i]);
        h.rooms.push_back(node);
        h.totalRooms++;
    }
    
    h.totalSpaces = 1;
    return h;
}

std::string formatSpaceBreadcrumb(const std::vector<std::string>& names) {
    std::ostringstream os;
    for (size_t i = 0; i < names.size(); i++) {
        if (i > 0) os << " > ";
        os << names[i];
    }
    return os.str();
}

std::string formatSpaceSuggestion(const SpaceNode& node) {
    return node.name.empty() ? node.roomId : node.name;
}

bool isRoomInSpace(const SpaceHierarchy& h, const std::string& roomId) {
    return std::find(h.children.begin(), h.children.end(), roomId) != h.children.end();
}

} // namespace progressive
