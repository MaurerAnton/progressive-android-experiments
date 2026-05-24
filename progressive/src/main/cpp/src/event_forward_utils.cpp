#include "progressive/event_forward_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string buildForwardRequest(const std::vector<std::string>& roomIds) {
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "buildForwardRequest" << R"("})";
    return oss.str();
}

std::string buildForwardContent(const std::string& eventId, const std::string& roomId) {
    if (eventId.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "buildForwardContent" << R"(","input_len":)" << eventId.size();
    size_t al=0, dg=0;
    for(char c : eventId) { if(std::isalpha(c)) al++; else if(std::isdigit(c)) dg++; }
    oss << R"(,"alpha":)" << al << R"(,"digits":)" << dg;
    auto b=eventId.find('{');
    if(b!=std::string::npos){
        auto e=eventId.find('}',b);
        if(e!=std::string::npos&&e-b>2)
            oss << R"(,"fragment":")" << eventId.substr(b+1, std::min(size_t(20), e-b-1)) << R"(")";
    }
    oss << "}";
    return oss.str();
}
