#include "progressive/user_directory_formatter.hpp"
#include <sstream>

namespace progressive {

std::string buildUserSearchBody(const std::string& q, int limit) {
    std::ostringstream os; os << R"({"search_term":")" << q << R"(","limit":)" << limit << "}"; return os.str();
}
std::vector<UserDirectoryItem> parseUserSearchResults(const std::string& json) {
    std::vector<UserDirectoryItem> items; size_t pos = 0;
    while ((pos = json.find("\"user_id\":\"", pos)) != std::string::npos) {
        pos += 11; auto end = json.find('"', pos); if (end == std::string::npos) break;
        UserDirectoryItem u; u.userId = json.substr(pos, end - pos);
        auto dn = json.find("\"display_name\":\"", end); if (dn != std::string::npos && dn - end < 200) {
            dn += 16; auto dEnd = json.find('"', dn); if (dEnd != std::string::npos) u.displayName = json.substr(dn, dEnd - dn);
        }
        items.push_back(u); pos = end + 1;
    }
    return items;
}
std::string formatUserDirectoryItem(const UserDirectoryItem& u) {
    return (u.displayName.empty() ? u.userId : u.displayName) + " (" + u.userId + ")";
}
} // namespace progressive
