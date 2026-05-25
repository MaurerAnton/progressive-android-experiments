#include "progressive/room_previewer.hpp"
#include <sstream>

std::string parseRoomPreview(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseRoomPreview"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getRoomHeroes(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getRoomHeroes"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getRoomTopic(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getRoomTopic"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatPreviewCard(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatPreviewCard"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
