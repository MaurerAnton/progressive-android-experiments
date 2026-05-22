#include "progressive/room_directory_utils.hpp"
#include <sstream>

namespace progressive {

RoomDirectoryEntry parseDirectoryEntry(const std::string& json) {
    RoomDirectoryEntry e;
    auto extract = [&](const std::string& key) -> std::string {
        auto p = json.find("\"" + key + "\":\"");
        if (p == std::string::npos) return "";
        p += key.size() + 4;
        auto end = json.find('"', p);
        if (end == std::string::npos) return "";
        return json.substr(p, end - p);
    };
    e.roomId = extract("room_id");
    e.name = extract("name");
    e.topic = extract("topic");
    e.avatarUrl = extract("avatar_url");
    auto membersPos = json.find("\"num_joined_members\":");
    if (membersPos != std::string::npos) {
        membersPos += 20;
        try { e.memberCount = std::stoi(json.substr(membersPos)); } catch(...) {}
    }
    return e;
}

std::string buildDirectoryFilter(const std::string& searchTerm, int limit, const std::string& server) {
    std::ostringstream os;
    os << R"({"limit":)" << limit;
    if (!searchTerm.empty()) os << R"(,"filter":{"generic_search_term":")" << searchTerm << R"("})";
    if (!server.empty()) os << R"(,"server":")" << server << R"(")";
    os << "}";
    return os.str();
}

std::vector<RoomDirectoryEntry> parseDirectoryList(const std::string& json) {
    std::vector<RoomDirectoryEntry> entries;
    size_t pos = 0;
    while (pos < json.size()) {
        auto rid = json.find("\"room_id\":\"", pos);
        if (rid == std::string::npos) break;
        auto end = json.find('}', rid);
        if (end == std::string::npos) break;
        entries.push_back(parseDirectoryEntry(json.substr(rid, end - rid + 1)));
        pos = end + 1;
    }
    return entries;
}

std::string formatDirectoryEntry(const RoomDirectoryEntry& e) {
    std::ostringstream os;
    os << (e.name.empty() ? e.roomId : e.name);
    os << " (" << e.memberCount << " members)";
    return os.str();
}

} // namespace progressive
