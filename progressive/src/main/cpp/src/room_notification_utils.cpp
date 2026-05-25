#include "progressive/room_notification_utils.hpp"
#include <sstream>

std::string buildRoomNotifOverride(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildRoomNotifOverride"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildRoomNotifRemove(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildRoomNotifRemove"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatNotifLevel(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatNotifLevel"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
