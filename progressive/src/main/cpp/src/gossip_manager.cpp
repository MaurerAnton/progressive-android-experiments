#include "progressive/gossip_manager.hpp"
std::string gossipKeyRequest(const std::string&) { return R"({"ok":true})"; }
std::string receiveGossip(const std::string&) { return R"({"ok":true})"; }
std::string getPendingGossips(const std::string&) { return R"({"ok":true})"; }
std::string cancelGossip(const std::string&) { return R"({"ok":true})"; }
std::string gossipCount(const std::string&) { return R"({"ok":true})"; }
