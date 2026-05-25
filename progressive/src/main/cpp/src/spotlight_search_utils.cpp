#include "progressive/spotlight_search_utils.hpp"
#include <sstream>

std::string searchAll(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"searchAll"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getRecentSearches(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getRecentSearches"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string clearRecent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"clearRecent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string indexContent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"indexContent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
