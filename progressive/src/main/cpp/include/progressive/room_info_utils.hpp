#pragma once
#include <string>
#include <cstdint>

namespace progressive {

struct RoomInfo {
    std::string roomId;
    std::string name;
    std::string topic;
    std::string avatarUrl;
    std::string joinRule;
    int memberCount = 0;
    bool isEncrypted = false;
    bool isDirect = false;
    int roomVersion = 1;
};

// Parse room info from /state or /summary
RoomInfo parseRoomInfo(const std::string& json, const std::string& roomId);

// Format room info for display header
std::string formatRoomHeader(const RoomInfo& info);

// Parse room display name from state events
std::string parseRoomDisplayName(const std::string& nameEvent, const std::string& canonicalAlias,
                                   const std::string& roomId, const std::vector<std::string>& memberNames);

} // namespace progressive
