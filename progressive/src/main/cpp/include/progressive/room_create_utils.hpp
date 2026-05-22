#pragma once
#include <string>

namespace progressive {

struct RoomCreateInfo {
    std::string creator;
    std::string roomVersion;
    bool isFederated = true;
    std::string predecessorRoomId;   // if upgraded
};

// Parse m.room.create state event
RoomCreateInfo parseRoomCreate(const std::string& json);

// Build room create request body
std::string buildCreateRoomRequest(const std::string& name, const std::string& topic,
                                     bool isDirect, const std::string& preset = "private_chat");

// Format room creation info
std::string formatRoomCreateInfo(const RoomCreateInfo& info);

} // namespace progressive
