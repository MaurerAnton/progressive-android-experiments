#include "progressive/message_send_queue.hpp"
std::string enqueueSend(const std::string&) { return R"({"ok":true})"; }
std::string dequeueSend(const std::string&) { return R"({"ok":true})"; }
std::string getQueueSize(const std::string&) { return R"({"ok":true})"; }
std::string cancelSend(const std::string&) { return R"({"ok":true})"; }
std::string retryFailed(const std::string&) { return R"({"ok":true})"; }
