#include "progressive/toast_notification_utils.hpp"
#include <sstream>

std::string buildToastMessage(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildToastMessage"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildDelayedToast(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildDelayedToast"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatToastTime(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatToastTime"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
