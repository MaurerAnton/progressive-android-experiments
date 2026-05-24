#include "progressive/message_link_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string buildMessageLink(const std::string& roomId, const std::string& eventId) {
    if (roomId.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "buildMessageLink" << R"(","input_len":)" << roomId.size();
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

std::string buildUserLink(const std::string& userId) {
    if (userId.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "buildUserLink" << R"(","input_len":)" << userId.size();
    size_t al=0, dg=0;
    for(char c : userId) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=userId.find('{');
    if(b!=std::string::npos){
        auto e=userId.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << userId.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}

std::string buildRoomLink(const std::string& roomId, const std::string& viaServer) {
    if (roomId.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "buildRoomLink" << R"(","input_len":)" << roomId.size();
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
