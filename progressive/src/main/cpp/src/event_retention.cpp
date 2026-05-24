#include "progressive/event_retention.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string shouldRetain(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "shouldRetain" << R"(","size":)" << json.size();
    size_t a=0,d=0;
    for(char c : json) { if(std::isalpha(c)) a++; else if(std::isdigit(c)) d++; }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << "}";
    return oss.str();
}

std::string getRetentionPeriod(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "getRetentionPeriod" << R"(","size":)" << json.size();
    size_t a=0,d=0;
    for(char c : json) { if(std::isalpha(c)) a++; else if(std::isdigit(c)) d++; }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << "}";
    return oss.str();
}

std::string pruneOldEvents(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "pruneOldEvents" << R"(","size":)" << json.size();
    size_t a=0,d=0;
    for(char c : json) { if(std::isalpha(c)) a++; else if(std::isdigit(c)) d++; }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << "}";
    return oss.str();
}

std::string calculateAge(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "calculateAge" << R"(","size":)" << json.size();
    size_t a=0,d=0;
    for(char c : json) { if(std::isalpha(c)) a++; else if(std::isdigit(c)) d++; }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << "}";
    return oss.str();
}

std::string buildRetentionPolicy(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "buildRetentionPolicy" << R"(","size":)" << json.size();
    size_t a=0,d=0;
    for(char c : json) { if(std::isalpha(c)) a++; else if(std::isdigit(c)) d++; }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << "}";
    return oss.str();
}

