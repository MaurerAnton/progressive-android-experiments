#include "progressive/thread_summary.hpp"
#include <sstream>

std::string getThreadCount(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getThreadCount"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getLatestThreadEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getLatestThreadEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isParticipating(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isParticipating"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatThreadPreview(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatThreadPreview"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
