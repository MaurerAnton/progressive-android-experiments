#include "progressive/room_forgot_utils.hpp"
#include <sstream>

std::string parseForgotten(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseForgotten"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string markAsForgotten(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"markAsForgotten"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string unforgetRoom(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"unforgetRoom"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isForgotten(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isForgotten"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
