#include "progressive/room_visibility_utils.hpp"
#include <sstream>

std::string parseVisibility(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseVisibility"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isVisibleRoom(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isVisibleRoom"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildVisibilityEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildVisibilityEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
