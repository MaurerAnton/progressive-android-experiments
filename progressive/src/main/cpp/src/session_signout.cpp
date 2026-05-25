#include "progressive/session_signout.hpp"
#include <sstream>

std::string signOutSession(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"signOutSession"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string clearLocalData(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"clearLocalData"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getSessionList(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getSessionList"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isCurrentSession(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isCurrentSession"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
