#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

struct AvatarInfo {
    std::string url;            // mxc:// or https:// URL
    std::string thumbnailUrl;   // resized version
    int width = 0;
    int height = 0;
    bool isDefault = false;     // no custom avatar set
    std::string initials;       // computed from display name (e.g. "AB")
};

// Build MXC avatar URL
std::string buildAvatarUrl(const std::string& mxcUrl, const std::string& homeserver);
std::string buildThumbnailUrl(const std::string& mxcUrl, const std::string& homeserver,
                                int width, int height, const std::string& method = "crop");

// Parse avatar info from member event or profile
AvatarInfo parseAvatarInfo(const std::string& mxcUrl, const std::string& displayName = "");

// Compute initials from display name or userId
std::string computeInitials(const std::string& displayName, const std::string& userId = "");

// Get a consistent color from userId (for avatar fallback)
std::string getAvatarColor(const std::string& userId);

// Format avatar for display (URL or initials fallback)
std::string formatAvatarSource(const AvatarInfo& info, const std::string& homeserver);

} // namespace progressive
