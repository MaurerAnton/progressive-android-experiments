#include "progressive/notification_sound_utils.hpp"
#include <sstream>

std::string getDefaultNotificationSound(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getDefaultNotificationSound"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getHighlightNotificationSound(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getHighlightNotificationSound"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildSoundTweak(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildSoundTweak"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
