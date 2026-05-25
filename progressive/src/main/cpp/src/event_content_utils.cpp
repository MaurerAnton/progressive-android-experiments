#include "progressive/event_content_utils.hpp"
#include <sstream>

std::string parseContentType(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseContentType"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string extractTextBody(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"extractTextBody"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string extractFormattedBody(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"extractFormattedBody"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string extractMediaUrl(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"extractMediaUrl"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
