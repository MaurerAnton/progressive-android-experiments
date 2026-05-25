#include "progressive/permission_validator.hpp"
#include <sstream>

std::string checkRoomPermission(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"checkRoomPermission"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getPowerLevel(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getPowerLevel"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string canSendMessage(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"canSendMessage"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string canInviteUser(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"canInviteUser"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
