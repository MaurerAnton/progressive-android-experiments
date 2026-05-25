#include "progressive/avatar_url_utils.hpp"
#include <sstream>

std::string parseAvatarUrl(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseAvatarUrl"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildAvatarUpdateEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildAvatarUpdateEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
