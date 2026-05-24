#include "progressive/room_notification_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string buildRoomNotifOverride(const std::string& roomId, const std::string& level) {
    if (roomId.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "buildRoomNotifOverride" << R"(","input_len":)" << roomId.size();
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

std::string buildRoomNotifRemove(const std::string& roomId) {
    if (roomId.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "buildRoomNotifRemove" << R"(","input_len":)" << roomId.size();
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

std::string formatNotifLevel(const std::string& level) {
    if (level.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "formatNotifLevel" << R"(","input_len":)" << level.size();
    size_t al=0, dg=0;
    for(char c : level) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=level.find('{');
    if(b!=std::string::npos){
        auto e=level.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << level.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}
