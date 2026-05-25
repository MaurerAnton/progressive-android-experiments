#include "progressive/sync_optimizer.hpp"
#include <sstream>

std::string mergeSyncResponses(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"mergeSyncResponses"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string dedupeEvents(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"dedupeEvents"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string prioritizeRoom(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"prioritizeRoom"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string shouldSkipRoom(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"shouldSkipRoom"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
