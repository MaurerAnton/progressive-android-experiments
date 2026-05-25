#include "progressive/event_hash_utils.hpp"
#include <sstream>

std::string verifyEventHash(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"verifyEventHash"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string hashEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"hashEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string computeSha256(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"computeSha256"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
