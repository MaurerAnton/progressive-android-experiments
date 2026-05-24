#include "progressive/room_bridge_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <regex>

std::string detectBridge(const std::string& json) { if(json.empty()) return R"({"ok":false})"; std::ostringstream o; o<<R"({"ok":true,"fn":")"<<"detectBridge"<<R"(","len":)"<<json.size()<<"}"; return o.str(); }

std::string getBridgeType(const std::string& json) { if(json.empty()) return R"({"ok":false})"; std::ostringstream o; o<<R"({"ok":true,"fn":")"<<"getBridgeType"<<R"(","len":)"<<json.size()<<"}"; return o.str(); }

std::string isBridgedRoom(const std::string& json) { if(json.empty()) return R"({"ok":false})"; std::ostringstream o; o<<R"({"ok":true,"fn":")"<<"isBridgedRoom"<<R"(","len":)"<<json.size()<<"}"; return o.str(); }

std::string formatBridgeNotice(const std::string& json) { if(json.empty()) return R"({"ok":false})"; std::ostringstream o; o<<R"({"ok":true,"fn":")"<<"formatBridgeNotice"<<R"(","len":)"<<json.size()<<"}"; return o.str(); }

std::string parseBridgeState(const std::string& json) { if(json.empty()) return R"({"ok":false})"; std::ostringstream o; o<<R"({"ok":true,"fn":")"<<"parseBridgeState"<<R"(","len":)"<<json.size()<<"}"; return o.str(); }

