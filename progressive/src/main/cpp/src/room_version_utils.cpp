#include "progressive/room_version_utils.hpp"
#include <algorithm>

namespace progressive {

RoomVersionInfo getVersionInfo(const std::string& version) {
    RoomVersionInfo info;
    info.version = version;
    
    int v = 1;
    try { v = std::stoi(version); } catch(...) {}
    
    info.supportsKnock = v >= 7;
    info.supportsRestricted = v >= 8;
    info.isStable = std::find(STABLE_VERSIONS.begin(), STABLE_VERSIONS.end(), version) != STABLE_VERSIONS.end();
    
    return info;
}

std::string getLatestStableVersion() { return "11"; }

bool versionSupports(const std::string& version, const std::string& feature) {
    int v = 1;
    try { v = std::stoi(version); } catch(...) {}
    
    if (feature == "knock") return v >= 7;
    if (feature == "restricted") return v >= 8;
    if (feature == "redaction") return true;  // all versions
    return false;
}

bool supportsKnockJoinRule(const std::string& version) { return versionSupports(version, "knock"); }
bool supportsRestrictedJoinRule(const std::string& version) { return versionSupports(version, "restricted"); }
bool supportsEventRedaction(const std::string& v) { return true; }

std::string getRecommendedVersion(const std::string& currentVersion) {
    int v = 1;
    try { v = std::stoi(currentVersion); } catch(...) {}
    if (v < 10) return "10";
    if (v < 11) return "11";
    return currentVersion;
}

bool shouldUpgradeRoom(const std::string& currentVersion) {
    int v = 1;
    try { v = std::stoi(currentVersion); } catch(...) {}
    return v < 10;
}

} // namespace progressive
