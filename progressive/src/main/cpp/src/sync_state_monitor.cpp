#include "progressive/sync_state_monitor.hpp"
#include <sstream>

std::string getSyncState(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getSyncState"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isSyncing(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isSyncing"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getSyncProgress(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getSyncProgress"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getLastSyncTime(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getLastSyncTime"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
