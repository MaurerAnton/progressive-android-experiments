#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

enum class DeviceTrustLevel {
    UNKNOWN = -1,
    UNVERIFIED = 0,
    VERIFIED = 1,
    BLOCKED = 2
};

struct DeviceTrustInfo {
    std::string deviceId;
    std::string userId;
    DeviceTrustLevel level = DeviceTrustLevel::UNKNOWN;
    bool isCrossSigningVerified = false;
    bool isLocallyVerified = false;
    std::string lastVerifiedDate;
};

// Compute trust level from verification flags
DeviceTrustLevel computeTrustLevel(bool crossSigningVerified, bool locallyVerified);

// Convert trust level to display string
std::string trustLevelToString(DeviceTrustLevel level);
std::string trustLevelToEmoji(DeviceTrustLevel level);
std::string trustLevelToColor(DeviceTrustLevel level);

// Parse device trust from crypto device info JSON
DeviceTrustInfo parseDeviceTrust(const std::string& deviceId, const std::string& userId,
                                   const std::string& cryptoJson);

// Check if device should show trust warning
bool needsTrustWarning(DeviceTrustLevel level);

// Format trust info for user display
std::string formatDeviceTrust(const DeviceTrustInfo& info);

} // namespace progressive
