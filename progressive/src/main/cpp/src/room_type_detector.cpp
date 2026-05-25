#include "progressive/room_type_detector.hpp"
#include <sstream>

std::string parseRoomType(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseRoomType"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isSpace(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isSpace"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isDirect(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isDirect"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isGroupRoom(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isGroupRoom"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
