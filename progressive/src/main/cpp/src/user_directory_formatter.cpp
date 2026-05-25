#include "progressive/user_directory_formatter.hpp"
#include <sstream>

std::string buildUserSearchBody(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildUserSearchBody"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatUserDirectoryItem(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatUserDirectoryItem"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
