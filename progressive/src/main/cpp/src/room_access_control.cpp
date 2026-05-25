#include "progressive/room_access_control.hpp"
#include <sstream>

std::string parseAccessRules(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseAccessRules"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string canUserJoin(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"canUserJoin"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isRestricted(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isRestricted"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildAccessEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildAccessEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
