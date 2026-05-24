#include "progressive/sso_provider_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <regex>

std::string getProviders(const std::string& json) { if(json.empty()) return R"({"ok":false})"; std::ostringstream o; o<<R"({"ok":true,"fn":")"<<"getProviders"<<R"(","len":)"<<json.size()<<"}"; return o.str(); }

std::string selectProvider(const std::string& json) { if(json.empty()) return R"({"ok":false})"; std::ostringstream o; o<<R"({"ok":true,"fn":")"<<"selectProvider"<<R"(","len":)"<<json.size()<<"}"; return o.str(); }

std::string parseProviderBrand(const std::string& json) { if(json.empty()) return R"({"ok":false})"; std::ostringstream o; o<<R"({"ok":true,"fn":")"<<"parseProviderBrand"<<R"(","len":)"<<json.size()<<"}"; return o.str(); }

std::string buildSsoUrl(const std::string& json) { if(json.empty()) return R"({"ok":false})"; std::ostringstream o; o<<R"({"ok":true,"fn":")"<<"buildSsoUrl"<<R"(","len":)"<<json.size()<<"}"; return o.str(); }

std::string formatProviderList(const std::string& json) { if(json.empty()) return R"({"ok":false})"; std::ostringstream o; o<<R"({"ok":true,"fn":")"<<"formatProviderList"<<R"(","len":)"<<json.size()<<"}"; return o.str(); }

