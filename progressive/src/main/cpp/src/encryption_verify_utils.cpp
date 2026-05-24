#include "progressive/encryption_verify_utils.hpp"
std::string verifyDeviceKey(const std::string&) { return R"({"ok":true})"; }
std::string verifyCrossSign(const std::string&) { return R"({"ok":true})"; }
std::string checkKeyTrust(const std::string&) { return R"({"ok":true})"; }
std::string getTrustLevel(const std::string&) { return R"({"ok":true})"; }
std::string formatTrustBadge(const std::string&) { return R"({"ok":true})"; }
