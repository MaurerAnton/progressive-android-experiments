#include "progressive/language_switcher.hpp"
#include <sstream>

std::string parseLanguage(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseLanguage"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getSupportedLanguages(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getSupportedLanguages"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string setAppLanguage(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"setAppLanguage"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getDefaultLanguage(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getDefaultLanguage"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
