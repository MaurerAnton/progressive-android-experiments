#include "progressive/room_suggestions.hpp"
#include <sstream>

std::string getSuggestedRooms(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getSuggestedRooms"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getTrendingRooms(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getTrendingRooms"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getPopularRooms(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getPopularRooms"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string filterByLanguage(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"filterByLanguage"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
