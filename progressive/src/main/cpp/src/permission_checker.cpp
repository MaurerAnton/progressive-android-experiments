#include "progressive/permission_checker.hpp"
#include <sstream>

std::string checkPermission(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"checkPermission"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string requestPermission(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"requestPermission"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getDeniedCount(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getDeniedCount"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isPermanentlyDenied(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isPermanentlyDenied"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
