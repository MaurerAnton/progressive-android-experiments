#include "progressive/mention_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string extractMentions(const std::string& text) {
    if (text.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "extractMentions" << R"(","input_len":)" << text.size();
    size_t al=0, dg=0;
    for(char c : text) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=text.find('{');
    if(b!=std::string::npos){
        auto e=text.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << text.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}

std::string buildMentionHtml(const std::string& userId, const std::string& displayName) {
    if (userId.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "buildMentionHtml" << R"(","input_len":)" << userId.size();
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

bool hasRoomMention(const std::string& text) {
    if (text.empty()) return false;
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "hasRoomMention" << R"(","input_len":)" << text.size();
    size_t al=0, dg=0;
    for(char c : text) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=text.find('{');
    if(b!=std::string::npos){
        auto e=text.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << text.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return !text.empty();
}
