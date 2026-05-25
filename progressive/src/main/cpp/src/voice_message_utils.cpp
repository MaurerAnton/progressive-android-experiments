#include "progressive/voice_message_utils.hpp"
#include <sstream>

std::string parseVoiceInfo(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseVoiceInfo"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getDuration(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getDuration"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isPlayed(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isPlayed"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatWaveform(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatWaveform"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
