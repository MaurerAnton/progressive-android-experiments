#include "progressive/room_sidebar.hpp"
#include <sstream>

std::string getSidebarOrder(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getSidebarOrder"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string pinRoom(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"pinRoom"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string unpinRoom(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"unpinRoom"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isPinned(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isPinned"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
