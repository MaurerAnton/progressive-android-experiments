#include "progressive/display_name_utils.hpp"
#include <sstream>

std::string parseDisplayName(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseDisplayName"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatDisplayName(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatDisplayName"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildDisplayNameUpdate(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildDisplayNameUpdate"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
