#include "progressive/room_upgrade_utils.hpp"
#include <sstream>

std::string parseUpgrade(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseUpgrade"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getReplacementRoom(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getReplacementRoom"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isTombstoned(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isTombstoned"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildUpgradeLink(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildUpgradeLink"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
