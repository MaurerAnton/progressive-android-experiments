#include "progressive/session_watchdog.hpp"
std::string checkHeartbeat(const std::string&) { return R"({"ok":true})"; }
std::string lastActivityAge(const std::string&) { return R"({"ok":true})"; }
std::string shouldTerminate(const std::string&) { return R"({"ok":true})"; }
std::string resetWatchdog(const std::string&) { return R"({"ok":true})"; }
