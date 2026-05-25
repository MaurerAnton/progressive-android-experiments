#include "progressive/notification_action_utils.hpp"
#include <sstream>

std::string parseActions(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseActions"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildNotificationActions(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildNotificationActions"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
