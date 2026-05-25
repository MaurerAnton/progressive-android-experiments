#include "progressive/room_mute_scheduler.hpp"
#include <sstream>

std::string scheduleMute(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"scheduleMute"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string cancelMute(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"cancelMute"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getMuteUntil(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getMuteUntil"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isMuted(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isMuted"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
