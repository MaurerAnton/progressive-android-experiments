#include "progressive/deeplink_parser.hpp"
#include <sstream>

std::string parseMatrixTo(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseMatrixTo"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string parseMatrixLink(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseMatrixLink"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isValidDeeplink(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isValidDeeplink"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string extractRoomId(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"extractRoomId"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
