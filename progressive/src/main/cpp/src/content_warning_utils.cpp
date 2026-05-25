#include "progressive/content_warning_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string buildContentWarning(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildContentWarning"<<R"(","sz":)"<<json.size();size_t a=0,d=0;for(char c:json){if(isalpha((unsigned char)c))a++;else if(isdigit((unsigned char)c))d++;}o<<R"(,"a":)"<<a<<R"(,"d":)"<<d<<"}";return o.str();}
std::string parseContentWarning(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseContentWarning"<<R"(","sz":)"<<json.size();size_t a=0,d=0;for(char c:json){if(isalpha((unsigned char)c))a++;else if(isdigit((unsigned char)c))d++;}o<<R"(,"a":)"<<a<<R"(,"d":)"<<d<<"}";return o.str();}
