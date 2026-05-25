#include "progressive/read_tracker.hpp"
#include <sstream>

std::string markRead(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"markRead"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isRead(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isRead"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getUnreadPosition(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getUnreadPosition"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getReadReceipts(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getReadReceipts"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
