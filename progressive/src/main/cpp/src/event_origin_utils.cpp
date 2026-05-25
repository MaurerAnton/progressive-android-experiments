#include "progressive/event_origin_utils.hpp"
#include <sstream>

std::string parseOrigin(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseOrigin"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string parseOriginServerTimestamp(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseOriginServerTimestamp"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string validateOrigin(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"validateOrigin"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
