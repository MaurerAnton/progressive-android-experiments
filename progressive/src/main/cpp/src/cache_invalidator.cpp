#include "progressive/cache_invalidator.hpp"
std::string invalidateByKey(const std::string&) { return R"({"ok":true})"; }
std::string invalidateByPrefix(const std::string&) { return R"({"ok":true})"; }
std::string isStale(const std::string&) { return R"({"ok":true})"; }
std::string getTtl(const std::string&) { return R"({"ok":true})"; }
std::string resetCache(const std::string&) { return R"({"ok":true})"; }
