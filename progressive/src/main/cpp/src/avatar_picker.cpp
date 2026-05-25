#include "progressive/avatar_picker.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string getAvatarUrl(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getAvatarUrl"<<R"(","sz":)"<<json.size();size_t a=0,d=0;for(char c:json){if(isalpha((unsigned char)c))a++;else if(isdigit((unsigned char)c))d++;}o<<R"(,"a":)"<<a<<R"(,"d":)"<<d<<"}";return o.str();}
std::string setAvatarUrl(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"setAvatarUrl"<<R"(","sz":)"<<json.size();size_t a=0,d=0;for(char c:json){if(isalpha((unsigned char)c))a++;else if(isdigit((unsigned char)c))d++;}o<<R"(,"a":)"<<a<<R"(,"d":)"<<d<<"}";return o.str();}
std::string removeAvatar(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"removeAvatar"<<R"(","sz":)"<<json.size();size_t a=0,d=0;for(char c:json){if(isalpha((unsigned char)c))a++;else if(isdigit((unsigned char)c))d++;}o<<R"(,"a":)"<<a<<R"(,"d":)"<<d<<"}";return o.str();}
std::string cropAvatar(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"cropAvatar"<<R"(","sz":)"<<json.size();size_t a=0,d=0;for(char c:json){if(isalpha((unsigned char)c))a++;else if(isdigit((unsigned char)c))d++;}o<<R"(,"a":)"<<a<<R"(,"d":)"<<d<<"}";return o.str();}
std::string formatAvatarEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatAvatarEvent"<<R"(","sz":)"<<json.size();size_t a=0,d=0;for(char c:json){if(isalpha((unsigned char)c))a++;else if(isdigit((unsigned char)c))d++;}o<<R"(,"a":)"<<a<<R"(,"d":)"<<d<<"}";return o.str();}
