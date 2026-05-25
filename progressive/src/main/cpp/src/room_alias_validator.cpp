#include "progressive/room_alias_validator.hpp"
#include <sstream>

std::string parseAlias(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseAlias"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isValidAlias(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isValidAlias"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isLocalAlias(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isLocalAlias"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getServerFromAlias(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getServerFromAlias"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
