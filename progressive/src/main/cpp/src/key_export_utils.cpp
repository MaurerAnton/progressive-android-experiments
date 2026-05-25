#include "progressive/key_export_utils.hpp"
#include <sstream>

std::string exportRoomKeys(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"exportRoomKeys"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string importRoomKeys(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"importRoomKeys"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string parseKeyFile(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseKeyFile"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string verifyKeyPassword(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"verifyKeyPassword"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
