#include "progressive/topic_parser.hpp"
#include <sstream>

std::string parseTopic(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseTopic"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string extractTopicLinks(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"extractTopicLinks"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatTopicHtml(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatTopicHtml"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string truncateTopic(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"truncateTopic"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
