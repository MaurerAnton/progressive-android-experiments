#include "progressive/avatar_utils.hpp"
#include <sstream>
#include <functional>

namespace progressive {

std::string buildAvatarUrl(const std::string& mxcUrl, const std::string& homeserver) {
    if (mxcUrl.empty()) return "";
    const std::string prefix = "mxc://";
    if (mxcUrl.compare(0, prefix.size(), prefix) != 0) return mxcUrl; // already HTTP
    
    auto slash = mxcUrl.find('/', prefix.size());
    if (slash == std::string::npos) return "";
    std::string server = mxcUrl.substr(prefix.size(), slash - prefix.size());
    std::string mediaId = mxcUrl.substr(slash + 1);
    
    std::string base = homeserver;
    while (!base.empty() && base.back() == '/') base.pop_back();
    
    return base + "/_matrix/media/v3/download/" + server + "/" + mediaId;
}

std::string buildThumbnailUrl(const std::string& mxcUrl, const std::string& homeserver,
                                int width, int height, const std::string& method) {
    if (mxcUrl.empty()) return "";
    std::string base = buildAvatarUrl(mxcUrl, homeserver);
    // Replace download with thumbnail
    auto dlPos = base.find("/download/");
    if (dlPos != std::string::npos) {
        base.replace(dlPos, 10, "/thumbnail/");
    }
    std::ostringstream os;
    os << base << "?width=" << width << "&height=" << height << "&method=" << method;
    return os.str();
}

AvatarInfo parseAvatarInfo(const std::string& url, const std::string& displayName) {
    AvatarInfo info;
    info.url = url;
    info.isDefault = url.empty();
    info.initials = computeInitials(displayName);
    return info;
}

std::string computeInitials(const std::string& displayName, const std::string& userId) {
    if (!displayName.empty()) {
        // Take first letter of first and last word
        auto first = displayName.find_first_not_of(" \t@");
        if (first == std::string::npos) return "?";
        std::string initials(1, displayName[first]);
        auto lastSpace = displayName.find_last_of(" \t");
        if (lastSpace != std::string::npos && lastSpace + 1 < displayName.size()) {
            initials += displayName[lastSpace + 1];
        }
        return initials;
    }
    if (!userId.empty() && userId.size() > 1) {
        return userId.substr(1, 1); // First letter after @
    }
    return "?";
}

std::string getAvatarColor(const std::string& userId) {
    size_t h = std::hash<std::string>{}(userId);
    const char* colors[] = {"#E53935","#1E88E5","#43A047","#FB8C00",
                             "#8E24AA","#00ACC1","#3949AB","#C0CA33"};
    return colors[h % 8];
}

std::string formatAvatarSource(const AvatarInfo& info, const std::string& homeserver) {
    if (!info.url.empty()) return buildAvatarUrl(info.url, homeserver);
    return ""; // Caller should render initials
}

} // namespace progressive
