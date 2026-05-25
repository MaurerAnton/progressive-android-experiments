#include "progressive/public_room_lister.hpp"
#include <sstream>

std::string parsePublicRooms(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parsePublicRooms"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string filterBySearch(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"filterBySearch"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string sortByPopularity(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"sortByPopularity"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string paginateRooms(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"paginateRooms"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
