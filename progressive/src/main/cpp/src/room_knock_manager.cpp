#include "progressive/room_knock_manager.hpp"
std::string sendKnock(const std::string&) { return R"({"ok":true})"; }
std::string parseKnockResponse(const std::string&) { return R"({"ok":true})"; }
std::string canKnock(const std::string&) { return R"({"ok":true})"; }
std::string getPendingKnocks(const std::string&) { return R"({"ok":true})"; }
std::string cancelKnock(const std::string&) { return R"({"ok":true})"; }
