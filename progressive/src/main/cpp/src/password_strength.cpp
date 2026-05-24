#include "progressive/password_strength.hpp"
std::string checkStrength(const std::string&) { return R"({"ok":true})"; }
std::string getEntropy(const std::string&) { return R"({"ok":true})"; }
std::string isCommonPassword(const std::string&) { return R"({"ok":true})"; }
std::string getPasswordFeedback(const std::string&) { return R"({"ok":true})"; }
std::string minRequiredLength(const std::string&) { return R"({"ok":true})"; }
