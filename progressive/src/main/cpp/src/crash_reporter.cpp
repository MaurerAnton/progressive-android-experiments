#include "progressive/crash_reporter.hpp"
#include <sstream>

std::string captureCrash(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"captureCrash"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatStackTrace(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatStackTrace"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getDeviceInfo(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getDeviceInfo"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildCrashReport(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildCrashReport"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
