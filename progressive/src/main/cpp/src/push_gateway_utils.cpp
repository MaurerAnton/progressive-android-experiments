#include "progressive/push_gateway_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string buildPusherRequest(const std::string& pushKey, const std::string& appId, const std::string& appName, const std::string& deviceName, const std::string& profileTag, const std::string& lang, const std::string& url, const std::string& format) {
    if (pushKey.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "buildPusherRequest" << R"(","input_len":)" << pushKey.size();
    size_t al=0, dg=0;
    for(char c : pushKey) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=pushKey.find('{');
    if(b!=std::string::npos){
        auto e=pushKey.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << pushKey.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}

std::string parsePusherResponse(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "parsePusherResponse" << R"(","input_len":)" << json.size();
    size_t al=0, dg=0;
    for(char c : json) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=json.find('{');
    if(b!=std::string::npos){
        auto e=json.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << json.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}
