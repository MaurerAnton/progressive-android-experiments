#include "progressive/room_leave_utils.hpp"
#include <sstream>

namespace progressive {
std::string buildLeaveRequest(const std::string& r) { return r.empty() ? "{}" : R"({"reason":")" + r + R"("})"; }
std::string buildKickRequest(const std::string& uid, const std::string& r) {
    std::ostringstream os; os << R"({"user_id":")" << uid << R"(")";
    if (!r.empty()) os << R"(,"reason":")" << r << R"(")";
    os << "}"; return os.str();
}
std::string buildBanRequest(const std::string& uid, const std::string& r) {
    std::ostringstream os; os << R"({"user_id":")" << uid << R"(","membership":"ban")";
    if (!r.empty()) os << R"(,"reason":")" << r << R"(")";
    os << "}"; return os.str();
}
std::string buildUnbanRequest(const std::string& uid) { return R"({"user_id":")" + uid + R"(","membership":"leave"})"; }
} // namespace progressive
