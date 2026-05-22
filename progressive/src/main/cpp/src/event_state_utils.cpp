#include "progressive/event_state_utils.hpp"
#include <sstream>

namespace progressive {

std::string parseStateKey(const std::string& json) {
    auto p = json.find("\"state_key\":\""); if (p == std::string::npos) return "";
    p += 13; auto e = json.find('"', p); return e != std::string::npos ? json.substr(p, e - p) : "";
}
bool isStateEvent(const std::string& json) { return json.find("\"state_key\"") != std::string::npos; }
std::string buildStateEvent(const std::string& type, const std::string& key, const std::string& content) {
    std::ostringstream os;
    os << R"({"type":")" << type << R"(","state_key":")" << key << R"(","content":)" << content << "}";
    return os.str();
}
std::string formatStateEvent(const std::string& type, const std::string& key, const std::string& sender) {
    if (type == "m.room.name") return sender + " changed the room name";
    if (type == "m.room.topic") return sender + " changed the topic";
    if (type == "m.room.avatar") return sender + " changed the avatar";
    if (type == "m.room.join_rules") return sender + " changed join rules";
    if (type == "m.room.guest_access") return sender + " changed guest access";
    if (type == "m.room.history_visibility") return sender + " changed history visibility";
    if (type == "m.room.power_levels") return sender + " changed power levels";
    return sender + " updated " + type;
}
} // namespace progressive
