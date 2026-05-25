#include "progressive/room_read_marker_utils.hpp"
#include <sstream>

std::string parseReadMarker(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseReadMarker"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string updateReadMarker(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"updateReadMarker"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildReadMarkerEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildReadMarkerEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
