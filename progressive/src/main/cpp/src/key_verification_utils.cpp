#include "progressive/key_verification_utils.hpp"
std::string parseVerificationStart(const std::string&) { return R"({"ok":true})"; }
std::string buildVerificationAccept(const std::string&) { return R"({"ok":true})"; }
std::string parseKeyVerificationMac(const std::string&) { return R"({"ok":true})"; }
std::string validateSasEmoji(const std::string&) { return R"({"ok":true})"; }
