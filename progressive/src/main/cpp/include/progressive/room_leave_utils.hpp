#pragma once
#include <string>

namespace progressive {

// Build leave room request (empty body)
std::string buildLeaveRequest(const std::string& reason = "");

// Build kick user request
std::string buildKickRequest(const std::string& userId, const std::string& reason = "");

// Build ban user request
std::string buildBanRequest(const std::string& userId, const std::string& reason = "");

// Build unban user request
std::string buildUnbanRequest(const std::string& userId);

} // namespace progressive
