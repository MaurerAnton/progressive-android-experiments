#include "progressive/media_scanner.hpp"
#include <sstream>

std::string scanMedia(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"scanMedia"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isSafeContent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isSafeContent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getMediaDimensions(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getMediaDimensions"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string detectBlurHash(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"detectBlurHash"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
