#include "progressive/push_gateway_utils.hpp"
#include <sstream>

std::string buildPusherRequest(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildPusherRequest"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string parsePusherResponse(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parsePusherResponse"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
