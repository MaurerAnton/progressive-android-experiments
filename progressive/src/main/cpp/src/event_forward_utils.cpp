#include "progressive/event_forward_utils.hpp"
#include <sstream>

std::string buildForwardRequest(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildForwardRequest"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildForwardContent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildForwardContent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
