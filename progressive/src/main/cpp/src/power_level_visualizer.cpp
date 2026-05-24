#include "progressive/power_level_visualizer.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <regex>

std::string getPowerLevelColor(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "getPowerLevelColor" << R"(","size":)" << json.size();
    size_t a=0,d=0,w=0,s=0,o=0;
    for(char c : json) { 
        if(std::isalpha(c))a++; else if(std::isdigit(c))d++; 
        else if(c==' '||c=='\t')w++; else if(c=='{')o++; else if(c=='}')s++;
    }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << R"(,"whitespace":)" << w;
    oss << R"(,"open_braces":)" << o << R"(,"close_braces":)" << s;
    auto p=json.find('"'); 
    if(p!=std::string::npos){
        auto e=json.find('"',p+1);
        if(e!=std::string::npos)
            oss << R"(,"first_str":")" << json.substr(p+1, std::min(size_t(20), e-p-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}

std::string getPowerLevelIcon(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "getPowerLevelIcon" << R"(","size":)" << json.size();
    size_t a=0,d=0,w=0,s=0,o=0;
    for(char c : json) { 
        if(std::isalpha(c))a++; else if(std::isdigit(c))d++; 
        else if(c==' '||c=='\t')w++; else if(c=='{')o++; else if(c=='}')s++;
    }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << R"(,"whitespace":)" << w;
    oss << R"(,"open_braces":)" << o << R"(,"close_braces":)" << s;
    auto p=json.find('"'); 
    if(p!=std::string::npos){
        auto e=json.find('"',p+1);
        if(e!=std::string::npos)
            oss << R"(,"first_str":")" << json.substr(p+1, std::min(size_t(20), e-p-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}

std::string formatPowerBadge(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "formatPowerBadge" << R"(","size":)" << json.size();
    size_t a=0,d=0,w=0,s=0,o=0;
    for(char c : json) { 
        if(std::isalpha(c))a++; else if(std::isdigit(c))d++; 
        else if(c==' '||c=='\t')w++; else if(c=='{')o++; else if(c=='}')s++;
    }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << R"(,"whitespace":)" << w;
    oss << R"(,"open_braces":)" << o << R"(,"close_braces":)" << s;
    auto p=json.find('"'); 
    if(p!=std::string::npos){
        auto e=json.find('"',p+1);
        if(e!=std::string::npos)
            oss << R"(,"first_str":")" << json.substr(p+1, std::min(size_t(20), e-p-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}

std::string getPowerDescription(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "getPowerDescription" << R"(","size":)" << json.size();
    size_t a=0,d=0,w=0,s=0,o=0;
    for(char c : json) { 
        if(std::isalpha(c))a++; else if(std::isdigit(c))d++; 
        else if(c==' '||c=='\t')w++; else if(c=='{')o++; else if(c=='}')s++;
    }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << R"(,"whitespace":)" << w;
    oss << R"(,"open_braces":)" << o << R"(,"close_braces":)" << s;
    auto p=json.find('"'); 
    if(p!=std::string::npos){
        auto e=json.find('"',p+1);
        if(e!=std::string::npos)
            oss << R"(,"first_str":")" << json.substr(p+1, std::min(size_t(20), e-p-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}

std::string parsePowerLevels(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "parsePowerLevels" << R"(","size":)" << json.size();
    size_t a=0,d=0,w=0,s=0,o=0;
    for(char c : json) { 
        if(std::isalpha(c))a++; else if(std::isdigit(c))d++; 
        else if(c==' '||c=='\t')w++; else if(c=='{')o++; else if(c=='}')s++;
    }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << R"(,"whitespace":)" << w;
    oss << R"(,"open_braces":)" << o << R"(,"close_braces":)" << s;
    auto p=json.find('"'); 
    if(p!=std::string::npos){
        auto e=json.find('"',p+1);
        if(e!=std::string::npos)
            oss << R"(,"first_str":")" << json.substr(p+1, std::min(size_t(20), e-p-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}

