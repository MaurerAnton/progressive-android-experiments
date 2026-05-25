#include "progressive/invite_validator.hpp"
#include <sstream>

std::string validateInvite(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"validateInvite"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isExpired(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isExpired"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string canAccept(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"canAccept"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getInviteDetails(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getInviteDetails"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
