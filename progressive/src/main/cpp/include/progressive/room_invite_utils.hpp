#pragma once
#include <string>
#include <vector>

namespace progressive {

struct InviteResult {
    std::string roomId;
    bool success = false;
    std::string error;
};

// Build invite request body
std::string buildInviteRequest(const std::string& userId, const std::string& reason = "");

// Parse invite response
InviteResult parseInviteResponse(const std::string& json, const std::string& roomId);

// Build 3PID invite request (email/phone)
std::string build3pidInviteRequest(const std::string& address, const std::string& medium,
                                      const std::string& idServer, const std::string& idAccessToken);

// Validate invite input
bool isValidInviteTarget(const std::string& input);  // MXID or email

// Format invite notice text
std::string formatInviteNotice(const std::string& inviterName, const std::string& roomName);

} // namespace progressive
