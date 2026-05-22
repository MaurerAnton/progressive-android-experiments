#pragma once
#include <string>

namespace progressive {

// Generate room avatar from MXID (first letter uppercase)
std::string generateRoomInitials(const std::string& roomName);

// Generate room color from room ID (consistent hash → color)
std::string getRoomColor(const std::string& roomId);

// Format room avatar URL with fallback
std::string formatRoomAvatarUrl(const std::string& mxcUrl, const std::string& homeserver,
                                  const std::string& roomName, const std::string& roomId);

// Check if room has a custom avatar
bool hasCustomRoomAvatar(const std::string& stateJson);

// Build room avatar change content
std::string buildRoomAvatarChange(const std::string& mxcUrl);

} // namespace progressive
