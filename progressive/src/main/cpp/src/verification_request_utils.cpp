#include "progressive/verification_request_utils.hpp"
std::string parseVerificationRequest(const std::string&) { return R"({"ok":true})"; }
std::string acceptRequest(const std::string&) { return R"({"ok":true})"; }
std::string declineRequest(const std::string&) { return R"({"ok":true})"; }
std::string getRequestStatus(const std::string&) { return R"({"ok":true})"; }
std::string formatRequestBanner(const std::string&) { return R"({"ok":true})"; }
