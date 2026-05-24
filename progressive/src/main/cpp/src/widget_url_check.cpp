#include "progressive/widget_url_check.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <regex>

std::string validateWidgetUrl(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "validateWidgetUrl" << R"(","size":)" << json.size();
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

std::string isAuthorizedDomain(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "isAuthorizedDomain" << R"(","size":)" << json.size();
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

std::string parseWidgetParams(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "parseWidgetParams" << R"(","size":)" << json.size();
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

std::string sanitizeWidgetHtml(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "sanitizeWidgetHtml" << R"(","size":)" << json.size();
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

std::string getWidgetCapabilities(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "getWidgetCapabilities" << R"(","size":)" << json.size();
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

