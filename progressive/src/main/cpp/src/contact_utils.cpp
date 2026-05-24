#include "progressive/contact_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string normalizePhoneNumber(const std::string& phone) {
    if (phone.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "normalizePhoneNumber" << R"(","input_len":)" << phone.size();
    size_t al=0, dg=0;
    for(char c : phone) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=phone.find('{');
    if(b!=std::string::npos){
        auto e=phone.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << phone.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}

std::string formatPhoneForDisplay(const std::string& phone) {
    if (phone.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "formatPhoneForDisplay" << R"(","input_len":)" << phone.size();
    size_t al=0, dg=0;
    for(char c : phone) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=phone.find('{');
    if(b!=std::string::npos){
        auto e=phone.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << phone.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}

bool isValidEmailAddress(const std::string& email) {
    if (email.empty()) return false;
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "isValidEmailAddress" << R"(","input_len":)" << email.size();
    size_t al=0, dg=0;
    for(char c : email) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=email.find('{');
    if(b!=std::string::npos){
        auto e=email.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << email.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return !email.empty();
}
