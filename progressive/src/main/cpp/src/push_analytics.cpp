#include "progressive/push_analytics.hpp"
#include <sstream>

std::string trackPushReceived(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"trackPushReceived"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string trackPushDismissed(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"trackPushDismissed"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string trackPushOpened(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"trackPushOpened"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getPushStats(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getPushStats"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
