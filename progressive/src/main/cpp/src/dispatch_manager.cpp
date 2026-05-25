#include "progressive/dispatch_manager.hpp"
#include <sstream>

std::string dispatchEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"dispatchEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string routeToHandler(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"routeToHandler"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string prioritySort(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"prioritySort"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string canHandle(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"canHandle"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
