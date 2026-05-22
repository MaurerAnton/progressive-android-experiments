#include "progressive/room_create_utils.hpp"
#include <sstream>

namespace progressive {

RoomCreateInfo parseRoomCreate(const std::string& json) {
    RoomCreateInfo c;
    auto extract = [&](const std::string& key) -> std::string {
        auto p = json.find("\"" + key + "\":\""); if (p == std::string::npos) return "";
        p += key.size() + 4; auto e = json.find('"', p);
        return e != std::string::npos ? json.substr(p, e - p) : "";
    };
    c.creator = extract("creator"); c.roomVersion = extract("room_version");
    c.isFederated = json.find("\"m.federate\":false") == std::string::npos;
    return c;
}
std::string buildCreateRoomRequest(const std::string& name, const std::string& topic,
                                     bool direct, const std::string& preset) {
    std::ostringstream os;
    os << R"({"name":")" << name << R"(","topic":")" << topic << R"(")";
    os << R"(,"preset":")" << preset << R"(")";
    os << R"(,"is_direct":)" << (direct ? "true" : "false");
    os << R"(,"visibility":"private")";
    os << "}"; return os.str();
}
std::string formatRoomCreateInfo(const RoomCreateInfo& i) {
    return "Created by " + i.creator + " (v" + i.roomVersion + ")";
}
} // namespace progressive
