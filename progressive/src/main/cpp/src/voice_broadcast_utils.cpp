#include "progressive/voice_broadcast_utils.hpp"
#include <sstream>

std::string parseVoiceBroadcastInfo(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseVoiceBroadcastInfo"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildVoiceBroadcastEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildVoiceBroadcastEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getVoiceBroadcastState(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getVoiceBroadcastState"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string canStartVoiceBroadcast(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"canStartVoiceBroadcast"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
