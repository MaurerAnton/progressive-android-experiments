#include "progressive/whois_utils.hpp"
#include <sstream>

std::string buildWhoisRequest(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildWhoisRequest"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatWhoisResult(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatWhoisResult"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
