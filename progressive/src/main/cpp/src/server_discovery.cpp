#include "progressive/server_discovery.hpp"
std::string discoverHomeserver(const std::string&) { return R"({"ok":true})"; }
std::string parseWellKnown(const std::string&) { return R"({"ok":true})"; }
std::string getIdentityServer(const std::string&) { return R"({"ok":true})"; }
std::string validateServerCert(const std::string&) { return R"({"ok":true})"; }
std::string cacheDiscovery(const std::string&) { return R"({"ok":true})"; }
