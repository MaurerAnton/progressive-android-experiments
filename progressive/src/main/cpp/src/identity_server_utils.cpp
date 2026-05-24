#include "progressive/identity_server_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string buildIdServerBindRequest(const std::string& sid, const std::string& clientSecret, const std::string& mxid) {
    if (sid.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "buildIdServerBindRequest" << R"(","input_len":)" << sid.size();
    size_t al=0, dg=0;
    for(char c : sid) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=sid.find('{');
    if(b!=std::string::npos){
        auto e=sid.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << sid.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}

std::string buildIdServerLookupRequest(const std::string& address, const std::string& medium) {
    if (address.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "buildIdServerLookupRequest" << R"(","input_len":)" << address.size();
    size_t al=0, dg=0;
    for(char c : address) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=address.find('{');
    if(b!=std::string::npos){
        auto e=address.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << address.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}
