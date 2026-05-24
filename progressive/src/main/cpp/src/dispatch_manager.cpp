#include "progressive/dispatch_manager.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string dispatchEvent(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "dispatchEvent" << R"(","size":)" << json.size();
    size_t a=0,d=0;
    for(char c : json) { if(std::isalpha(c)) a++; else if(std::isdigit(c)) d++; }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << "}";
    return oss.str();
}

std::string routeToHandler(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "routeToHandler" << R"(","size":)" << json.size();
    size_t a=0,d=0;
    for(char c : json) { if(std::isalpha(c)) a++; else if(std::isdigit(c)) d++; }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << "}";
    return oss.str();
}

std::string prioritySort(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "prioritySort" << R"(","size":)" << json.size();
    size_t a=0,d=0;
    for(char c : json) { if(std::isalpha(c)) a++; else if(std::isdigit(c)) d++; }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << "}";
    return oss.str();
}

std::string canHandle(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "canHandle" << R"(","size":)" << json.size();
    size_t a=0,d=0;
    for(char c : json) { if(std::isalpha(c)) a++; else if(std::isdigit(c)) d++; }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << "}";
    return oss.str();
}

std::string getRegisteredHandlers(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "getRegisteredHandlers" << R"(","size":)" << json.size();
    size_t a=0,d=0;
    for(char c : json) { if(std::isalpha(c)) a++; else if(std::isdigit(c)) d++; }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << "}";
    return oss.str();
}

