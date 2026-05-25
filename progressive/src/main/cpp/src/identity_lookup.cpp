#include "progressive/identity_lookup.hpp"
#include <sstream>

std::string lookupEmail(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"lookupEmail"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string lookupPhone(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"lookupPhone"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getHashedId(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getHashedId"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string parseLookupResult(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseLookupResult"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
