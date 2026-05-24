#include "progressive/notification_sound_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string getDefaultNotificationSound() {
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "getDefaultNotificationSound" << R"(","params":"none"})";
    return oss.str();
}

std::string getHighlightNotificationSound() {
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "getHighlightNotificationSound" << R"(","params":"none"})";
    return oss.str();
}

std::string buildSoundTweak(const std::string& soundUri) {
    if (soundUri.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "buildSoundTweak" << R"(","input_len":)" << soundUri.size();
    size_t al=0, dg=0;
    for(char c : soundUri) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=soundUri.find('{');
    if(b!=std::string::npos){
        auto e=soundUri.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << soundUri.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}
