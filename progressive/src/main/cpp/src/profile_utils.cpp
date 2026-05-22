#include "progressive/profile_utils.hpp"
#include <sstream>

namespace progressive {

std::string buildProfileRequest() { return "{}"; }

UserProfile parseProfile(const std::string& json, const std::string& userId) {
    UserProfile p;
    p.userId = userId;
    auto extract = [&](const std::string& key) -> std::string {
        auto pos = json.find("\"" + key + "\":\"");
        if (pos == std::string::npos) return "";
        pos += key.size() + 4;
        auto end = json.find('"', pos);
        if (end == std::string::npos) return "";
        return json.substr(pos, end - pos);
    };
    p.displayName = extract("displayname");
    if (p.displayName.empty()) p.displayName = extract("display_name");
    p.avatarUrl = extract("avatar_url");
    p.isDefault = p.displayName.empty() && p.avatarUrl.empty();
    return p;
}

std::string buildProfileUpdate(const UserProfile& p) {
    std::ostringstream os;
    os << "{";
    if (!p.displayName.empty()) os << R"("displayname":")" << p.displayName << R"(")";
    if (!p.avatarUrl.empty()) {
        if (!p.displayName.empty()) os << ",";
        os << R"("avatar_url":")" << p.avatarUrl << R"(")";
    }
    os << "}";
    return os.str();
}

std::string buildDisplayNameChange(const std::string& name) {
    return R"({"displayname":")" + name + R"("})";
}

std::string buildAvatarUrlChange(const std::string& url) {
    return R"({"avatar_url":")" + url + R"("})";
}

std::string formatProfileSummary(const UserProfile& p) {
    std::ostringstream os;
    os << (p.displayName.empty() ? p.userId : p.displayName);
    if (!p.avatarUrl.empty()) os << " [avatar set]";
    return os.str();
}

} // namespace progressive
