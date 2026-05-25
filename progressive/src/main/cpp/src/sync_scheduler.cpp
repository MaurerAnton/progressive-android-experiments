#include "progressive/sync_scheduler.hpp"
#include <sstream>

std::string scheduleNextSync(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"scheduleNextSync"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getRetryDelay(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getRetryDelay"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string shouldSyncNow(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"shouldSyncNow"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string resetSyncTimer(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"resetSyncTimer"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
