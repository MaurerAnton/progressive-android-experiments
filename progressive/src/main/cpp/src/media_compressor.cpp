#include "progressive/media_compressor.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string compressImage(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "compressImage" << R"(","size":)" << json.size();
    size_t a=0,d=0;
    for(char c : json) { if(std::isalpha(c)) a++; else if(std::isdigit(c)) d++; }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << "}";
    return oss.str();
}

std::string compressVideo(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "compressVideo" << R"(","size":)" << json.size();
    size_t a=0,d=0;
    for(char c : json) { if(std::isalpha(c)) a++; else if(std::isdigit(c)) d++; }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << "}";
    return oss.str();
}

std::string getCompressionRatio(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "getCompressionRatio" << R"(","size":)" << json.size();
    size_t a=0,d=0;
    for(char c : json) { if(std::isalpha(c)) a++; else if(std::isdigit(c)) d++; }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << "}";
    return oss.str();
}

std::string originalSize(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "originalSize" << R"(","size":)" << json.size();
    size_t a=0,d=0;
    for(char c : json) { if(std::isalpha(c)) a++; else if(std::isdigit(c)) d++; }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << "}";
    return oss.str();
}

std::string compressedSize(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "compressedSize" << R"(","size":)" << json.size();
    size_t a=0,d=0;
    for(char c : json) { if(std::isalpha(c)) a++; else if(std::isdigit(c)) d++; }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << "}";
    return oss.str();
}

