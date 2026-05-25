#include "progressive/room_bridge_utils.hpp"
#include <sstream>

std::string detectBridge(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"detectBridge"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getBridgeType(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getBridgeType"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isBridgedRoom(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isBridgedRoom"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatBridgeNotice(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatBridgeNotice"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
