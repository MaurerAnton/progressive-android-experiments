#include "progressive/room_state_event_utils.hpp"

namespace progressive {
std::string buildRoomNameEvent(const std::string& n) { return R"({"name":")" + n + R"("})"; }
std::string buildRoomTopicEvent(const std::string& t) { return R"({"topic":")" + t + R"("})"; }
std::string buildGuestAccessEvent(const std::string& a) { return R"({"guest_access":")" + a + R"("})"; }
std::string buildHistoryVisibilityEvent(const std::string& v) { return R"({"history_visibility":")" + v + R"("})"; }
std::string buildJoinRulesEvent(const std::string& r) { return R"({"join_rule":")" + r + R"("})"; }
} // namespace progressive
