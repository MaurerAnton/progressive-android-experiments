#include "progressive/room_tag_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string parseRoomTags(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "parseRoomTags" << R"(","input_len":)" << json.size();
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

std::string addRoomTag(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "addRoomTag" << R"(","input_len":)" << json.size();
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

std::string removeRoomTag(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "removeRoomTag" << R"(","input_len":)" << json.size();
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

std::string buildTagEvent(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "buildTagEvent" << R"(","input_len":)" << json.size();
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

