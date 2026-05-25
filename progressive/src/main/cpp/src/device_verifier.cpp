#include "progressive/device_verifier.hpp"
#include <sstream>

std::string startVerification(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"startVerification"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string acceptVerification(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"acceptVerification"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string cancelVerification(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"cancelVerification"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string parseVerificationEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseVerificationEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
