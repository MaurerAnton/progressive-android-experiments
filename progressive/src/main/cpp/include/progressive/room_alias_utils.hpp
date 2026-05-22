#pragma once
#include <string>
#include <vector>

namespace progressive {

struct RoomAliasInfo {
    std::string alias;          // "#room:server" 
    std::string roomId;         // resolved room ID
    std::vector<std::string> servers;  // via servers
    bool isValid = false;
};

// Resolve room alias to room ID
std::string buildAliasResolveRequest(const std::string& alias);

// Parse alias resolution response
RoomAliasInfo parseAliasResolve(const std::string& json, const std::string& alias);

// Validate alias format (#name:server)
bool isValidAlias(const std::string& alias);
bool isAliasUrl(const std::string& url);
std::string extractServerFromAlias(const std::string& alias);

// Build room alias creation request
std::string buildAliasCreateRequest(const std::string& alias);

// Parse aliases list from room state
std::vector<std::string> parseRoomAliases(const std::string& stateJson);

// Format alias for display
std::string formatAlias(const std::string& alias);

// Generate join URL from alias
std::string buildAliasJoinUrl(const std::string& alias);

} // namespace progressive
