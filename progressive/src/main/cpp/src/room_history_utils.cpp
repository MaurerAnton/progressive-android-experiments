#include "progressive/room_history_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

bool canUserReadHistory(const std::string& visibility, const std::string& membership, bool wasInvited) {
    if (visibility.empty()) return false;
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "canUserReadHistory" << R"(","input_len":)" << visibility.size();
    size_t al=0, dg=0;
    for(char c : visibility) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=visibility.find('{');
    if(b!=std::string::npos){
        auto e=visibility.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << visibility.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return !visibility.empty();
}

std::string getDefaultHistoryVisibility() {
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "getDefaultHistoryVisibility" << R"(","params":"none"})";
    return oss.str();
}

std::string formatHistoryVisibility(const std::string& vis) {
    if (vis.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "formatHistoryVisibility" << R"(","input_len":)" << vis.size();
    size_t al=0, dg=0;
    for(char c : vis) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=vis.find('{');
    if(b!=std::string::npos){
        auto e=vis.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << vis.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}
