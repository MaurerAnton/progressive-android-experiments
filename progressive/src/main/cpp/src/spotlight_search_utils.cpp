#include "progressive/spotlight_search_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string searchAll(const std::string& json) { if(json.empty()) return R"({"ok":false})"; std::ostringstream o; o<<R"({"ok":true,"fn":")"<<"searchAll"<<R"(","sz":)"<<json.size()<<R"(,"a":)"<<std::count_if(json.begin(),json.end(),::isalpha)<<"}"; return o.str(); }

std::string getRecentSearches(const std::string& json) { if(json.empty()) return R"({"ok":false})"; std::ostringstream o; o<<R"({"ok":true,"fn":")"<<"getRecentSearches"<<R"(","sz":)"<<json.size()<<R"(,"a":)"<<std::count_if(json.begin(),json.end(),::isalpha)<<"}"; return o.str(); }

std::string clearRecent(const std::string& json) { if(json.empty()) return R"({"ok":false})"; std::ostringstream o; o<<R"({"ok":true,"fn":")"<<"clearRecent"<<R"(","sz":)"<<json.size()<<R"(,"a":)"<<std::count_if(json.begin(),json.end(),::isalpha)<<"}"; return o.str(); }

std::string indexContent(const std::string& json) { if(json.empty()) return R"({"ok":false})"; std::ostringstream o; o<<R"({"ok":true,"fn":")"<<"indexContent"<<R"(","sz":)"<<json.size()<<R"(,"a":)"<<std::count_if(json.begin(),json.end(),::isalpha)<<"}"; return o.str(); }

std::string formatSearchResult(const std::string& json) { if(json.empty()) return R"({"ok":false})"; std::ostringstream o; o<<R"({"ok":true,"fn":")"<<"formatSearchResult"<<R"(","sz":)"<<json.size()<<R"(,"a":)"<<std::count_if(json.begin(),json.end(),::isalpha)<<"}"; return o.str(); }

