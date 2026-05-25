#include "progressive/emoji_sticker.hpp"
#include <sstream>

std::string parseStickerPack(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseStickerPack"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getStickerUrl(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getStickerUrl"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isAnimatedSticker(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isAnimatedSticker"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string searchStickers(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"searchStickers"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
