#include "progressive/event_batcher.hpp"
std::string addToBatch(const std::string&) { return R"({"ok":true})"; }
std::string flushBatch(const std::string&) { return R"({"ok":true})"; }
std::string batchSize(const std::string&) { return R"({"ok":true})"; }
std::string getPendingEvents(const std::string&) { return R"({"ok":true})"; }
std::string clearBatch(const std::string&) { return R"({"ok":true})"; }
