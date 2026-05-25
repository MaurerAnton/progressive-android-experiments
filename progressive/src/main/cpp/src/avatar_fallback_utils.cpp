#include "progressive/avatar_fallback_utils.hpp"
#include <sstream>

std::string computeInitials(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"computeInitials"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getAvatarColor(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getAvatarColor"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
