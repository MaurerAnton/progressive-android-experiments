#include "progressive/qr_code_utils.hpp"
#include <sstream>

std::string parseQrContent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseQrContent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildQrPayload(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildQrPayload"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string verifyQrSignature(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"verifyQrSignature"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isValidQrFormat(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isValidQrFormat"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
