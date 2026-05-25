#include "progressive/room_kicker.hpp"
#include <sstream>

std::string canKickUser(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"canKickUser"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildKickEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildKickEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string parseKickResponse(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseKickResponse"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getKickReason(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getKickReason"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
