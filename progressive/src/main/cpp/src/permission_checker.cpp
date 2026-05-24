#include "progressive/permission_checker.hpp"
std::string checkPermission(const std::string&) { return R"({"ok":true})"; }
std::string requestPermission(const std::string&) { return R"({"ok":true})"; }
std::string getDeniedCount(const std::string&) { return R"({"ok":true})"; }
std::string isPermanentlyDenied(const std::string&) { return R"({"ok":true})"; }
