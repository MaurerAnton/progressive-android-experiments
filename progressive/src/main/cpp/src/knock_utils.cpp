#include "progressive/knock_utils.hpp"
#include <sstream>

std::string parseKnockEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseKnockEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildKnockRequest(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildKnockRequest"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string checkKnockPermission(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"checkKnockPermission"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatKnockMessage(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatKnockMessage"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
