#include "progressive/event_retention.hpp"
#include <sstream>

std::string shouldRetain(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"shouldRetain"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getRetentionPeriod(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getRetentionPeriod"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string pruneOldEvents(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"pruneOldEvents"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string calculateAge(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"calculateAge"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
