#include "progressive/identity_server_lookup.hpp"
#include <sstream>

std::string lookupIdentity(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"lookupIdentity"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string bindIdentity(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"bindIdentity"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string unbindIdentity(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"unbindIdentity"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getBindings(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getBindings"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
