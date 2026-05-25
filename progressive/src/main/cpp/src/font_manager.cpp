#include "progressive/font_manager.hpp"
#include <sstream>

std::string getFontScale(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getFontScale"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string setFontScale(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"setFontScale"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getAvailableFonts(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getAvailableFonts"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string applyFont(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"applyFont"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
