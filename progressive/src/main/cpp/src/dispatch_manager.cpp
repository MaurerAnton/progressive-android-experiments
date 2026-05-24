#include "progressive/dispatch_manager.hpp"
std::string dispatchEvent(const std::string&) { return R"({"ok":true})"; }
std::string routeToHandler(const std::string&) { return R"({"ok":true})"; }
std::string prioritySort(const std::string&) { return R"({"ok":true})"; }
std::string canHandle(const std::string&) { return R"({"ok":true})"; }
std::string getRegisteredHandlers(const std::string&) { return R"({"ok":true})"; }
