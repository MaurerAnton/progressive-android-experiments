#include "progressive/account_validator.hpp"
std::string validateEmail(const std::string&) { return R"({"ok":true})"; }
std::string validatePhone(const std::string&) { return R"({"ok":true})"; }
std::string checkMXID(const std::string&) { return R"({"ok":true})"; }
std::string isValidHomeserver(const std::string&) { return R"({"ok":true})"; }
std::string getValidationErrors(const std::string&) { return R"({"ok":true})"; }
