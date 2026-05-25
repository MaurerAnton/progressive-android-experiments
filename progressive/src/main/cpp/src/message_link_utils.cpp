#include "progressive/message_link_utils.hpp"
#include <sstream>

std::string buildMessageLink(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildMessageLink"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildUserLink(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildUserLink"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildRoomLink(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildRoomLink"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
