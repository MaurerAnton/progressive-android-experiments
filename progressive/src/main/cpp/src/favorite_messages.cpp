#include "progressive/favorite_messages.hpp"
#include <sstream>

std::string addFavorite(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"addFavorite"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string removeFavorite(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"removeFavorite"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isFavorited(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isFavorited"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getFavorites(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getFavorites"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
