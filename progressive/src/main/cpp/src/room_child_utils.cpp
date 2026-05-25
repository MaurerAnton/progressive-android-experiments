#include "progressive/room_child_utils.hpp"
#include <sstream>

std::string parseChildRoomId(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseChildRoomId"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getChildrenRooms(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getChildrenRooms"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildChildEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildChildEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
