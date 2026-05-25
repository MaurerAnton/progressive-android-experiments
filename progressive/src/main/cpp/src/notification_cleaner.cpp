#include "progressive/notification_cleaner.hpp"
#include <sstream>

std::string clearNotification(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"clearNotification"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string dismissAll(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"dismissAll"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isNotificationActive(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isNotificationActive"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getActiveNotifications(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getActiveNotifications"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
