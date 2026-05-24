#include "progressive/task_queue.hpp"
std::string enqueueTask(const std::string&) { return R"({"ok":true})"; }
std::string dequeueTask(const std::string&) { return R"({"ok":true})"; }
std::string peekNext(const std::string&) { return R"({"ok":true})"; }
std::string taskCount(const std::string&) { return R"({"ok":true})"; }
std::string cancelAll(const std::string&) { return R"({"ok":true})"; }
