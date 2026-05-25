#include "progressive/user_badge_utils.hpp"
#include <sstream>

std::string getUserBadge(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getUserBadge"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getPowerBadge(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getPowerBadge"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isAdmin(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isAdmin"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isModerator(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isModerator"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
