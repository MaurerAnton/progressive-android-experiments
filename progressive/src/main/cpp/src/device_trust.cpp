#include "progressive/device_trust.hpp"
#include <sstream>

namespace progressive {

DeviceTrustLevel computeTrustLevel(bool crossSigningVerified, bool locallyVerified) {
    if (crossSigningVerified) return DeviceTrustLevel::VERIFIED;
    if (locallyVerified) return DeviceTrustLevel::VERIFIED;
    return DeviceTrustLevel::UNVERIFIED;
}

std::string trustLevelToString(DeviceTrustLevel level) {
    switch (level) {
        case DeviceTrustLevel::VERIFIED:   return "Verified";
        case DeviceTrustLevel::UNVERIFIED: return "Unverified";
        case DeviceTrustLevel::BLOCKED:    return "Blocked";
        default: return "Unknown";
    }
}

std::string trustLevelToEmoji(DeviceTrustLevel level) {
    switch (level) {
        case DeviceTrustLevel::VERIFIED:   return "\xF0\x9F\x9F\xA2"; // green
        case DeviceTrustLevel::UNVERIFIED: return "\xF0\x9F\x9F\xA1"; // yellow
        case DeviceTrustLevel::BLOCKED:    return "\xF0\x9F\x94\xB4"; // red
        default: return "\xE2\x9A\xAA";   // white
    }
}

std::string trustLevelToColor(DeviceTrustLevel level) {
    switch (level) {
        case DeviceTrustLevel::VERIFIED:   return "#4CAF50";
        case DeviceTrustLevel::UNVERIFIED: return "#FF9800";
        case DeviceTrustLevel::BLOCKED:    return "#F44336";
        default: return "#9E9E9E";
    }
}

DeviceTrustInfo parseDeviceTrust(const std::string& deviceId, const std::string& userId,
                                   const std::string& cryptoJson) {
    DeviceTrustInfo info;
    info.deviceId = deviceId;
    info.userId = userId;
    
    info.isCrossSigningVerified = cryptoJson.find("\"cross_signing_verified\":true") != std::string::npos;
    info.isLocallyVerified = cryptoJson.find("\"locally_verified\":true") != std::string::npos;
    info.level = computeTrustLevel(info.isCrossSigningVerified, info.isLocallyVerified);
    
    if (cryptoJson.find("\"blocked\":true") != std::string::npos) info.level = DeviceTrustLevel::BLOCKED;
    return info;
}

bool needsTrustWarning(DeviceTrustLevel level) {
    return level == DeviceTrustLevel::UNVERIFIED || level == DeviceTrustLevel::UNKNOWN;
}

std::string formatDeviceTrust(const DeviceTrustInfo& info) {
    std::ostringstream os;
    os << trustLevelToEmoji(info.level) << " " << trustLevelToString(info.level);
    if (info.isCrossSigningVerified) os << " (cross-signing)";
    return os.str();
}



bool isDeviceBlacklisted(const std::string& deviceId, const std::vector<std::string>& blacklist) {
    return std::find(blacklist.begin(), blacklist.end(), deviceId) != blacklist.end();
}

DeviceTrustLevel getWorstTrust(const std::vector<DeviceTrustInfo>& devices) {
    DeviceTrustLevel worst = DeviceTrustLevel::VERIFIED;
    for (const auto& d : devices) {
        if (d.level < worst) worst = d.level;
    }
    return worst;
}

bool areAllDevicesVerified(const std::vector<DeviceTrustInfo>& devices) {
    return getWorstTrust(devices) == DeviceTrustLevel::VERIFIED;
}

std::string buildTrustBadge(const DeviceTrustInfo& info) {
    return trustLevelToEmoji(info.level) + " " + trustLevelToString(info.level);
}

} // namespace progressive
