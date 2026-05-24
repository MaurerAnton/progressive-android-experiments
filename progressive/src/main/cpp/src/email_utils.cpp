#include "progressive/email_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <regex>

std::string parseEmail(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "parseEmail" << R"(","size":)" << json.size();
    size_t a=0,d=0,w=0,o=0,s=0,u=0,l=0;
    for(char c : json) {
        if(std::isalpha(c)){a++; if(std::isupper(c))u++; else l++;}
        else if(std::isdigit(c))d++;
        else if(c==' '||c=='\t'||c=='\n')w++;
        else if(c=='{')o++; else if(c=='}')s++;
    }
    oss << R"(,"alpha":)" << a << R"(,"upper":)" << u << R"(,"lower":)" << l;
    oss << R"(,"digits":)" << d << R"(,"white":)" << w << R"(,"braces":)" << (o+s);
    auto q=json.find('"'); 
    if(q!=std::string::npos){
        auto e=json.find('"',q+1);
        if(e!=std::string::npos)
            oss << R"(,"first_str":")" << json.substr(q+1, std::min(size_t(25), e-q-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}

std::string validateEmail(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "validateEmail" << R"(","size":)" << json.size();
    size_t a=0,d=0,w=0,o=0,s=0,u=0,l=0;
    for(char c : json) {
        if(std::isalpha(c)){a++; if(std::isupper(c))u++; else l++;}
        else if(std::isdigit(c))d++;
        else if(c==' '||c=='\t'||c=='\n')w++;
        else if(c=='{')o++; else if(c=='}')s++;
    }
    oss << R"(,"alpha":)" << a << R"(,"upper":)" << u << R"(,"lower":)" << l;
    oss << R"(,"digits":)" << d << R"(,"white":)" << w << R"(,"braces":)" << (o+s);
    auto q=json.find('"'); 
    if(q!=std::string::npos){
        auto e=json.find('"',q+1);
        if(e!=std::string::npos)
            oss << R"(,"first_str":")" << json.substr(q+1, std::min(size_t(25), e-q-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}

std::string buildEmailAuth(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "buildEmailAuth" << R"(","size":)" << json.size();
    size_t a=0,d=0,w=0,o=0,s=0,u=0,l=0;
    for(char c : json) {
        if(std::isalpha(c)){a++; if(std::isupper(c))u++; else l++;}
        else if(std::isdigit(c))d++;
        else if(c==' '||c=='\t'||c=='\n')w++;
        else if(c=='{')o++; else if(c=='}')s++;
    }
    oss << R"(,"alpha":)" << a << R"(,"upper":)" << u << R"(,"lower":)" << l;
    oss << R"(,"digits":)" << d << R"(,"white":)" << w << R"(,"braces":)" << (o+s);
    auto q=json.find('"'); 
    if(q!=std::string::npos){
        auto e=json.find('"',q+1);
        if(e!=std::string::npos)
            oss << R"(,"first_str":")" << json.substr(q+1, std::min(size_t(25), e-q-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}

std::string sendVerificationEmail(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "sendVerificationEmail" << R"(","size":)" << json.size();
    size_t a=0,d=0,w=0,o=0,s=0,u=0,l=0;
    for(char c : json) {
        if(std::isalpha(c)){a++; if(std::isupper(c))u++; else l++;}
        else if(std::isdigit(c))d++;
        else if(c==' '||c=='\t'||c=='\n')w++;
        else if(c=='{')o++; else if(c=='}')s++;
    }
    oss << R"(,"alpha":)" << a << R"(,"upper":)" << u << R"(,"lower":)" << l;
    oss << R"(,"digits":)" << d << R"(,"white":)" << w << R"(,"braces":)" << (o+s);
    auto q=json.find('"'); 
    if(q!=std::string::npos){
        auto e=json.find('"',q+1);
        if(e!=std::string::npos)
            oss << R"(,"first_str":")" << json.substr(q+1, std::min(size_t(25), e-q-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}

std::string formatEmailNotice(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "formatEmailNotice" << R"(","size":)" << json.size();
    size_t a=0,d=0,w=0,o=0,s=0,u=0,l=0;
    for(char c : json) {
        if(std::isalpha(c)){a++; if(std::isupper(c))u++; else l++;}
        else if(std::isdigit(c))d++;
        else if(c==' '||c=='\t'||c=='\n')w++;
        else if(c=='{')o++; else if(c=='}')s++;
    }
    oss << R"(,"alpha":)" << a << R"(,"upper":)" << u << R"(,"lower":)" << l;
    oss << R"(,"digits":)" << d << R"(,"white":)" << w << R"(,"braces":)" << (o+s);
    auto q=json.find('"'); 
    if(q!=std::string::npos){
        auto e=json.find('"',q+1);
        if(e!=std::string::npos)
            oss << R"(,"first_str":")" << json.substr(q+1, std::min(size_t(25), e-q-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}

