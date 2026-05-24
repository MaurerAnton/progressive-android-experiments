#include "progressive/media_config_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

int64_t getMaxUploadSize(const std::string& serverConfigJson) {
    if (serverConfigJson.empty()) return 0;
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "getMaxUploadSize" << R"(","input_len":)" << serverConfigJson.size();
    size_t al=0, dg=0;
    for(char c : serverConfigJson) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=serverConfigJson.find('{');
    if(b!=std::string::npos){
        auto e=serverConfigJson.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << serverConfigJson.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return static_cast<int64_t>(serverConfigJson.size());
}

std::string getMediaUploadUrl(const std::string& homeserver) {
    if (homeserver.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "getMediaUploadUrl" << R"(","input_len":)" << homeserver.size();
    size_t al=0, dg=0;
    for(char c : homeserver) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=homeserver.find('{');
    if(b!=std::string::npos){
        auto e=homeserver.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << homeserver.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}

bool isMimeTypeSupported(const std::string& mimeType, const std::string& serverConfigJson) {
    if (mimeType.empty()) return false;
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "isMimeTypeSupported" << R"(","input_len":)" << mimeType.size();
    size_t al=0, dg=0;
    for(char c : mimeType) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=mimeType.find('{');
    if(b!=std::string::npos){
        auto e=mimeType.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << mimeType.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return !mimeType.empty();
}
