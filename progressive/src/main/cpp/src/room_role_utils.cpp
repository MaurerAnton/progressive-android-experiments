#include "progressive/room_role_utils.hpp"
#include <sstream>

std::string parseSuggestedRole(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseSuggestedRole"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getDefaultRole(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getDefaultRole"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildRoleSuggestion(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildRoleSuggestion"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
