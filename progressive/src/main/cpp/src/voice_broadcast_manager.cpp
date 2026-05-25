#include "progressive/voice_broadcast_manager.hpp"
#include <sstream>

std::string startBroadcast(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"startBroadcast"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string stopBroadcast(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"stopBroadcast"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getBroadcastState(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getBroadcastState"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getListenerCount(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getListenerCount"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
