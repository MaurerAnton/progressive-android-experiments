#include "progressive/custom_emoji_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string buildCustomEmojiImage(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildCustomEmojiImage"<<R"(","sz":)"<<json.size();size_t a=0,d=0;for(char c:json){if(isalpha((unsigned char)c))a++;else if(isdigit((unsigned char)c))d++;}o<<R"(,"a":)"<<a<<R"(,"d":)"<<d<<"}";return o.str();}
std::string extractEmojiShortcode(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"extractEmojiShortcode"<<R"(","sz":)"<<json.size();size_t a=0,d=0;for(char c:json){if(isalpha((unsigned char)c))a++;else if(isdigit((unsigned char)c))d++;}o<<R"(,"a":)"<<a<<R"(,"d":)"<<d<<"}";return o.str();}
std::string replaceEmojiShortcodes(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"replaceEmojiShortcodes"<<R"(","sz":)"<<json.size();size_t a=0,d=0;for(char c:json){if(isalpha((unsigned char)c))a++;else if(isdigit((unsigned char)c))d++;}o<<R"(,"a":)"<<a<<R"(,"d":)"<<d<<"}";return o.str();}
