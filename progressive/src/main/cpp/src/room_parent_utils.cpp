#include "progressive/room_parent_utils.hpp"
#include <sstream>

std::string parseParentRoomId(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseParentRoomId"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getParentSpaces(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getParentSpaces"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildParentEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildParentEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
