#include "progressive/identity_server_utils.hpp"
#include <sstream>

std::string buildIdServerBindRequest(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildIdServerBindRequest"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildIdServerLookupRequest(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildIdServerLookupRequest"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
