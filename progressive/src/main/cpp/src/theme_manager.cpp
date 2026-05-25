#include "progressive/theme_manager.hpp"
#include <sstream>

std::string parseTheme(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseTheme"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string applyDarkMode(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"applyDarkMode"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getAccentColor(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getAccentColor"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isSystemTheme(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isSystemTheme"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
