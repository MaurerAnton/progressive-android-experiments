#include "progressive/thirdparty_invite.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <regex>

std::string parseThirdpartyInvite(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "parseThirdpartyInvite" << R"(","size":)" << json.size();
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

std::string acceptThirdparty(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "acceptThirdparty" << R"(","size":)" << json.size();
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

std::string rejectThirdparty(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "rejectThirdparty" << R"(","size":)" << json.size();
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

std::string isValidThirdparty(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "isValidThirdparty" << R"(","size":)" << json.size();
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

std::string formatThirdpartyNotice(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "formatThirdpartyNotice" << R"(","size":)" << json.size();
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

