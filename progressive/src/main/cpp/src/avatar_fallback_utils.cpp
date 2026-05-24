#include "progressive/avatar_fallback_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string computeInitials(const std::string& name, int maxChars) {
    if (name.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "computeInitials" << R"(","input_len":)" << name.size();
    size_t al=0, dg=0;
    for(char c : name) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=name.find('{');
    if(b!=std::string::npos){
        auto e=name.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << name.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}

std::string getAvatarColor(const std::string& id) {
    if (id.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "getAvatarColor" << R"(","input_len":)" << id.size();
    size_t al=0, dg=0;
    for(char c : id) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=id.find('{');
    if(b!=std::string::npos){
        auto e=id.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << id.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}
