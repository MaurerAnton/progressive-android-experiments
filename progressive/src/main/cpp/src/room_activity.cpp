#include "progressive/room_activity.hpp"
#include <sstream>

std::string getLastActivity(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getLastActivity"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isRoomActive(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isRoomActive"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getMessageFrequency(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getMessageFrequency"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatActivitySummary(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatActivitySummary"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
