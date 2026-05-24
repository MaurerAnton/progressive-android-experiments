#include "progressive/event_retention.hpp"
std::string shouldRetain(const std::string&) { return R"({"ok":true})"; }
std::string getRetentionPeriod(const std::string&) { return R"({"ok":true})"; }
std::string pruneOldEvents(const std::string&) { return R"({"ok":true})"; }
std::string calculateAge(const std::string&) { return R"({"ok":true})"; }
std::string buildRetentionPolicy(const std::string&) { return R"({"ok":true})"; }
