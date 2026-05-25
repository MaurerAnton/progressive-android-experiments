#include "progressive/task_queue.hpp"
#include <sstream>

std::string enqueueTask(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"enqueueTask"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string dequeueTask(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"dequeueTask"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string peekNext(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"peekNext"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string taskCount(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"taskCount"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
