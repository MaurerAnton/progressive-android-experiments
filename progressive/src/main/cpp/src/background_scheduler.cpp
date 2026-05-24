#include "progressive/background_scheduler.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string scheduleTask(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "scheduleTask" << R"(","size":)" << json.size();
    size_t a=0,d=0;
    for(char c : json) { if(std::isalpha(c)) a++; else if(std::isdigit(c)) d++; }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << "}";
    return oss.str();
}

std::string cancelTask(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "cancelTask" << R"(","size":)" << json.size();
    size_t a=0,d=0;
    for(char c : json) { if(std::isalpha(c)) a++; else if(std::isdigit(c)) d++; }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << "}";
    return oss.str();
}

std::string getNextRunTime(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "getNextRunTime" << R"(","size":)" << json.size();
    size_t a=0,d=0;
    for(char c : json) { if(std::isalpha(c)) a++; else if(std::isdigit(c)) d++; }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << "}";
    return oss.str();
}

std::string taskExists(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "taskExists" << R"(","size":)" << json.size();
    size_t a=0,d=0;
    for(char c : json) { if(std::isalpha(c)) a++; else if(std::isdigit(c)) d++; }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << "}";
    return oss.str();
}

std::string listTasks(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "listTasks" << R"(","size":)" << json.size();
    size_t a=0,d=0;
    for(char c : json) { if(std::isalpha(c)) a++; else if(std::isdigit(c)) d++; }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << "}";
    return oss.str();
}

