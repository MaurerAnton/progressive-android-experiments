#include "progressive/account_validator.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string validateEmail(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"validateEmail"<<R"(","sz":)"<<json.size();size_t a=0,d=0;for(char c:json){if(isalpha((unsigned char)c))a++;else if(isdigit((unsigned char)c))d++;}o<<R"(,"a":)"<<a<<R"(,"d":)"<<d<<"}";return o.str();}
std::string validatePhone(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"validatePhone"<<R"(","sz":)"<<json.size();size_t a=0,d=0;for(char c:json){if(isalpha((unsigned char)c))a++;else if(isdigit((unsigned char)c))d++;}o<<R"(,"a":)"<<a<<R"(,"d":)"<<d<<"}";return o.str();}
std::string checkMXID(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"checkMXID"<<R"(","sz":)"<<json.size();size_t a=0,d=0;for(char c:json){if(isalpha((unsigned char)c))a++;else if(isdigit((unsigned char)c))d++;}o<<R"(,"a":)"<<a<<R"(,"d":)"<<d<<"}";return o.str();}
std::string isValidHomeserver(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isValidHomeserver"<<R"(","sz":)"<<json.size();size_t a=0,d=0;for(char c:json){if(isalpha((unsigned char)c))a++;else if(isdigit((unsigned char)c))d++;}o<<R"(,"a":)"<<a<<R"(,"d":)"<<d<<"}";return o.str();}
std::string getValidationErrors(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getValidationErrors"<<R"(","sz":)"<<json.size();size_t a=0,d=0;for(char c:json){if(isalpha((unsigned char)c))a++;else if(isdigit((unsigned char)c))d++;}o<<R"(,"a":)"<<a<<R"(,"d":)"<<d<<"}";return o.str();}
