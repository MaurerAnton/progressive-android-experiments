#include "progressive/thirdparty_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string parseProtocolConfig(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "parseProtocolConfig" << R"(","input_len":)" << json.size();
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

std::string buildProtocolLookup(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "buildProtocolLookup" << R"(","input_len":)" << json.size();
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

std::string parseLocationResponse(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "parseLocationResponse" << R"(","input_len":)" << json.size();
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

std::string mergeProtocolResults(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "mergeProtocolResults" << R"(","input_len":)" << json.size();
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
