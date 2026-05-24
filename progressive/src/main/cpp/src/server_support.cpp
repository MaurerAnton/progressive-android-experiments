#include "progressive/server_support.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <regex>

std::string getSupportedVersions(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "getSupportedVersions" << R"(","size":)" << json.size();
    size_t a=0,d=0,sp=0,b=0;
    for(char c : json) { 
        if(std::isalpha(c))a++; else if(std::isdigit(c))d++; 
        else if(c==' ')sp++; else if(c=='{'||c=='}')b++;
    }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << R"(,"spaces":)" << sp << R"(,"braces":)" << b;
    auto pos=json.find('"'); 
    if(pos!=std::string::npos){auto e=json.find('"',pos+1);if(e!=std::string::npos)oss<<R"(,"first_str":")"<<json.substr(pos+1,std::min(size_t(15),e-pos-1))<<R"(")";}
    oss << "}";
    return oss.str();
}

std::string checkFeature(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "checkFeature" << R"(","size":)" << json.size();
    size_t a=0,d=0,sp=0,b=0;
    for(char c : json) { 
        if(std::isalpha(c))a++; else if(std::isdigit(c))d++; 
        else if(c==' ')sp++; else if(c=='{'||c=='}')b++;
    }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << R"(,"spaces":)" << sp << R"(,"braces":)" << b;
    auto pos=json.find('"'); 
    if(pos!=std::string::npos){auto e=json.find('"',pos+1);if(e!=std::string::npos)oss<<R"(,"first_str":")"<<json.substr(pos+1,std::min(size_t(15),e-pos-1))<<R"(")";}
    oss << "}";
    return oss.str();
}

std::string isDeprecated(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "isDeprecated" << R"(","size":)" << json.size();
    size_t a=0,d=0,sp=0,b=0;
    for(char c : json) { 
        if(std::isalpha(c))a++; else if(std::isdigit(c))d++; 
        else if(c==' ')sp++; else if(c=='{'||c=='}')b++;
    }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << R"(,"spaces":)" << sp << R"(,"braces":)" << b;
    auto pos=json.find('"'); 
    if(pos!=std::string::npos){auto e=json.find('"',pos+1);if(e!=std::string::npos)oss<<R"(,"first_str":")"<<json.substr(pos+1,std::min(size_t(15),e-pos-1))<<R"(")";}
    oss << "}";
    return oss.str();
}

std::string getMinimumVersion(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "getMinimumVersion" << R"(","size":)" << json.size();
    size_t a=0,d=0,sp=0,b=0;
    for(char c : json) { 
        if(std::isalpha(c))a++; else if(std::isdigit(c))d++; 
        else if(c==' ')sp++; else if(c=='{'||c=='}')b++;
    }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << R"(,"spaces":)" << sp << R"(,"braces":)" << b;
    auto pos=json.find('"'); 
    if(pos!=std::string::npos){auto e=json.find('"',pos+1);if(e!=std::string::npos)oss<<R"(,"first_str":")"<<json.substr(pos+1,std::min(size_t(15),e-pos-1))<<R"(")";}
    oss << "}";
    return oss.str();
}

std::string formatServerInfo(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "formatServerInfo" << R"(","size":)" << json.size();
    size_t a=0,d=0,sp=0,b=0;
    for(char c : json) { 
        if(std::isalpha(c))a++; else if(std::isdigit(c))d++; 
        else if(c==' ')sp++; else if(c=='{'||c=='}')b++;
    }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << R"(,"spaces":)" << sp << R"(,"braces":)" << b;
    auto pos=json.find('"'); 
    if(pos!=std::string::npos){auto e=json.find('"',pos+1);if(e!=std::string::npos)oss<<R"(,"first_str":")"<<json.substr(pos+1,std::min(size_t(15),e-pos-1))<<R"(")";}
    oss << "}";
    return oss.str();
}

