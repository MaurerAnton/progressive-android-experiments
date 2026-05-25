#include "progressive/typing_indicator.hpp"
#include <sstream>

std::string parseTypingEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseTypingEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isTyping(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isTyping"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getTypingTimeout(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getTypingTimeout"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatTypingList(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatTypingList"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
