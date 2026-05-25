#include "progressive/thread_utils.hpp"
#include <sstream>

std::string parseThreadInfo(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseThreadInfo"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildThreadRoot(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildThreadRoot"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getThreadParent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getThreadParent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string countThreadMessages(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"countThreadMessages"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
