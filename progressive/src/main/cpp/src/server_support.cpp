#include "progressive/server_support.hpp"
#include <sstream>

std::string getSupportedVersions(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getSupportedVersions"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string checkFeature(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"checkFeature"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isDeprecated(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isDeprecated"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getMinimumVersion(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getMinimumVersion"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
