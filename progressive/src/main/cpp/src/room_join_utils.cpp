#include "progressive/room_join_utils.hpp"
#include <sstream>

namespace progressive {

std::string buildJoinRequest(const std::vector<std::string>& via) {
    std::ostringstream os; os << "{";
    if (!via.empty()) { os << R"("via":[)"; for (size_t i = 0; i < via.size(); i++) { if (i > 0) os << ","; os << R"(")" << via[i] << R"(")"; } os << "]"; }
    os << "}"; return os.str();
}
std::string buildJoinByAlias(const std::string& alias, const std::vector<std::string>& via) {
    std::ostringstream os; os << R"({"room_alias":")" << alias << R"(")";
    if (!via.empty()) { os << R"(,"via":[)"; for (size_t i = 0; i < via.size(); i++) { if (i > 0) os << ","; os << R"(")" << via[i] << R"(")"; } os << "]"; }
    os << "}"; return os.str();
}
std::string parseJoinResponse(const std::string& json) {
    auto p = json.find("\"room_id\":\""); if (p == std::string::npos) return "";
    p += 11; auto e = json.find('"', p); return e != std::string::npos ? json.substr(p, e - p) : "";
}
std::string formatJoinConfirmation(const std::string& rid) { return "Joined " + rid; }

} // namespace progressive
