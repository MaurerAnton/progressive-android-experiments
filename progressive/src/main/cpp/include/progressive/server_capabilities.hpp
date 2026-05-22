#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace progressive {

struct ServerCapability {
    std::string key;
    bool enabled = false;
    std::unordered_map<std::string, std::string> params;
};

struct ServerCapabilities {
    bool supportsThreads = false;
    bool supportsPolls = false;
    bool supportsLocation = false;
    bool supportsGroups = false;
    bool supportsSSO = false;
    bool supportsPasswordLogin = false;
    bool supportsTokenRefresh = false;
    std::vector<std::string> loginFlows;
    std::vector<std::string> roomVersions;
    std::unordered_map<std::string, ServerCapability> custom;
};

// Parse /versions API response
std::vector<std::string> parseServerVersions(const std::string& json);

// Parse /capabilities API response
ServerCapabilities parseCapabilities(const std::string& json);

// Check individual capabilities
bool supportsLoginFlow(const ServerCapabilities& caps, const std::string& flow);

// Get default room version
std::string getDefaultRoomVersion(const ServerCapabilities& caps);

// Format capabilities for display
std::string formatCapabilities(const ServerCapabilities& caps);

} // namespace progressive
