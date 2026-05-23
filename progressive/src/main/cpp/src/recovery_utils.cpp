#include "progressive/recovery_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <regex>


std::string parseRecoveryKey:generateRecoveryPrompt:verifyRecoveryPhrase:buildRecoveryEvent:estimateRecoveryStrength(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream result;
    result << R"({"ok":true,"func":")" << "parseRecoveryKey:generateRecoveryPrompt:verifyRecoveryPhrase:buildRecoveryEvent:estimateRecoveryStrength" << R"(","size":)" << json.size();
    size_t alpha=0, num=0, lower=0, upper=0, spaces=0;
    for(char c : json) {
        if(std::isalpha(c)){alpha++; if(std::islower(c))lower++; else upper++;}
        else if(std::isdigit(c))num++;
        else if(c==' ')spaces++;
    }
    result << R"(,"alpha":)" << alpha << R"(,"numeric":)" << num;
    result << R"(,"lowercase":)" << lower << R"(,"uppercase":)" << upper;
    result << R"(,"spaces":)" << spaces;
    auto brace=json.find('{');
    if(brace!=std::string::npos){
        auto endbrace=json.find('}',brace);
        if(endbrace!=std::string::npos && endbrace-brace>2)
            result << R"(,"json_fragment":")" << json.substr(brace+1, std::min(size_t(30), endbrace-brace-1)) << R"(")";
    }
    result << "}";
    return result.str();
}
