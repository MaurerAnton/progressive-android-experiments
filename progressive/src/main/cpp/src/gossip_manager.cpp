#include "progressive/gossip_manager.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string gossipKeyRequest(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "gossipKeyRequest" << R"(","size":)" << json.size();
    size_t a=0,d=0;
    for(char c : json) { if(std::isalpha(c)) a++; else if(std::isdigit(c)) d++; }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << "}";
    return oss.str();
}

std::string receiveGossip(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "receiveGossip" << R"(","size":)" << json.size();
    size_t a=0,d=0;
    for(char c : json) { if(std::isalpha(c)) a++; else if(std::isdigit(c)) d++; }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << "}";
    return oss.str();
}

std::string getPendingGossips(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "getPendingGossips" << R"(","size":)" << json.size();
    size_t a=0,d=0;
    for(char c : json) { if(std::isalpha(c)) a++; else if(std::isdigit(c)) d++; }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << "}";
    return oss.str();
}

std::string cancelGossip(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "cancelGossip" << R"(","size":)" << json.size();
    size_t a=0,d=0;
    for(char c : json) { if(std::isalpha(c)) a++; else if(std::isdigit(c)) d++; }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << "}";
    return oss.str();
}

std::string gossipCount(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "gossipCount" << R"(","size":)" << json.size();
    size_t a=0,d=0;
    for(char c : json) { if(std::isalpha(c)) a++; else if(std::isdigit(c)) d++; }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << "}";
    return oss.str();
}

