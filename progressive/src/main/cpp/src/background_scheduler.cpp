#include "progressive/background_scheduler.hpp"
std::string scheduleTask(const std::string&) { return R"({"ok":true})"; }
std::string cancelTask(const std::string&) { return R"({"ok":true})"; }
std::string getNextRunTime(const std::string&) { return R"({"ok":true})"; }
std::string taskExists(const std::string&) { return R"({"ok":true})"; }
std::string listTasks(const std::string&) { return R"({"ok":true})"; }
