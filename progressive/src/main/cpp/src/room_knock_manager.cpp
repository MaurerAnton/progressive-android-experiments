#include "progressive/room_knock_manager.hpp"
#include <sstream>

std::string sendKnock(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"sendKnock"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string parseKnockResponse(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseKnockResponse"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string canKnock(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"canKnock"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getPendingKnocks(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getPendingKnocks"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
