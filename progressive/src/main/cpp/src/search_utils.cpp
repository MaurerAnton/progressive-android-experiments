#include "progressive/search_utils.hpp"
#include <sstream>

std::string tokenizeQuery(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"tokenizeQuery"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildSearchBody(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildSearchBody"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string parseSearchResults(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseSearchResults"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string rankSearchResults(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"rankSearchResults"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
