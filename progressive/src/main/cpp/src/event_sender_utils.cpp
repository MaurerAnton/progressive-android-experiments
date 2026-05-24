#include "progressive/event_sender_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string getSenderDisplayName(const std::string& eventJson, const std::string& fallback) {
    if (eventJson.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "getSenderDisplayName" << R"(","input_len":)" << eventJson.size();
    size_t al=0, dg=0;
    for(char c : eventJson) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=eventJson.find('{');
    if(b!=std::string::npos){
        auto e=eventJson.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << eventJson.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}

bool isOwnEvent(const std::string& senderId, const std::string& myUserId) {
    if (senderId.empty()) return false;
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "isOwnEvent" << R"(","input_len":)" << senderId.size();
    size_t al=0, dg=0;
    for(char c : senderId) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=senderId.find('{');
    if(b!=std::string::npos){
        auto e=senderId.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << senderId.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return !senderId.empty();
}

std::string formatSenderAvatarUrl(const std::string& mxcUrl, const std::string& homeserver, int size) {
    if (mxcUrl.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "formatSenderAvatarUrl" << R"(","input_len":)" << mxcUrl.size();
    size_t al=0, dg=0;
    for(char c : mxcUrl) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=mxcUrl.find('{');
    if(b!=std::string::npos){
        auto e=mxcUrl.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << mxcUrl.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}
