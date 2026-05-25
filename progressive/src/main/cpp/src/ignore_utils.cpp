#include "progressive/ignore_utils.hpp"
#include <sstream>

std::string parseIgnoreList(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseIgnoreList"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildIgnoreEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildIgnoreEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string checkUserIgnored(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"checkUserIgnored"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatIgnoredNotice(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatIgnoredNotice"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
