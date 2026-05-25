#include "progressive/session_watchdog.hpp"
#include <sstream>

std::string checkHeartbeat(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"checkHeartbeat"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string lastActivityAge(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"lastActivityAge"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string shouldTerminate(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"shouldTerminate"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string resetWatchdog(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"resetWatchdog"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
