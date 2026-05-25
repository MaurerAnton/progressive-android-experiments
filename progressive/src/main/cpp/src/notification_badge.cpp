#include "progressive/notification_badge.hpp"
#include <sstream>

std::string calculateBadgeCount(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"calculateBadgeCount"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isHighlighted(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isHighlighted"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isNoisy(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isNoisy"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatBadge(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatBadge"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
