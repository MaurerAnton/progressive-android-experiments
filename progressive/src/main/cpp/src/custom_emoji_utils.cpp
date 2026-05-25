#include "progressive/custom_emoji_utils.hpp"
#include <sstream>

std::string buildCustomEmojiImage(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildCustomEmojiImage"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string extractEmojiShortcode(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"extractEmojiShortcode"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string replaceEmojiShortcodes(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"replaceEmojiShortcodes"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
