#include "progressive/room_knock_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string buildKnockRequest(const std::string& roomId, const std::vector<std::string>& via, const std::string& reason) {
    if (roomId.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "buildKnockRequest" << R"(","input_len":)" << roomId.size();
    size_t al=0, dg=0;
    for(char c : roomId) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=roomId.find('{');
    if(b!=std::string::npos){
        auto e=roomId.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << roomId.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}

std::string formatKnockResponse(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "formatKnockResponse" << R"(","input_len":)" << json.size();
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
