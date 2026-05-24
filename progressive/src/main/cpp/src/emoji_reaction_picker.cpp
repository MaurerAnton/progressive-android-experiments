#include "progressive/emoji_reaction_picker.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <regex>

std::string getFrequentEmojis(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "getFrequentEmojis" << R"(","size":)" << json.size();
    size_t a=0,d=0,sp=0,b=0;
    for(char c : json) { 
        if(std::isalpha(c))a++; else if(std::isdigit(c))d++; 
        else if(c==' ')sp++; else if(c=='{'||c=='}')b++;
    }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << R"(,"spaces":)" << sp << R"(,"braces":)" << b;
    auto pos=json.find('"'); 
    if(pos!=std::string::npos){auto e=json.find('"',pos+1);if(e!=std::string::npos)oss<<R"(,"first_str":")"<<json.substr(pos+1,std::min(size_t(15),e-pos-1))<<R"(")";}
    oss << "}";
    return oss.str();
}

std::string searchEmojis(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "searchEmojis" << R"(","size":)" << json.size();
    size_t a=0,d=0,sp=0,b=0;
    for(char c : json) { 
        if(std::isalpha(c))a++; else if(std::isdigit(c))d++; 
        else if(c==' ')sp++; else if(c=='{'||c=='}')b++;
    }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << R"(,"spaces":)" << sp << R"(,"braces":)" << b;
    auto pos=json.find('"'); 
    if(pos!=std::string::npos){auto e=json.find('"',pos+1);if(e!=std::string::npos)oss<<R"(,"first_str":")"<<json.substr(pos+1,std::min(size_t(15),e-pos-1))<<R"(")";}
    oss << "}";
    return oss.str();
}

std::string getEmojiCategory(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "getEmojiCategory" << R"(","size":)" << json.size();
    size_t a=0,d=0,sp=0,b=0;
    for(char c : json) { 
        if(std::isalpha(c))a++; else if(std::isdigit(c))d++; 
        else if(c==' ')sp++; else if(c=='{'||c=='}')b++;
    }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << R"(,"spaces":)" << sp << R"(,"braces":)" << b;
    auto pos=json.find('"'); 
    if(pos!=std::string::npos){auto e=json.find('"',pos+1);if(e!=std::string::npos)oss<<R"(,"first_str":")"<<json.substr(pos+1,std::min(size_t(15),e-pos-1))<<R"(")";}
    oss << "}";
    return oss.str();
}

std::string isCustomEmoji(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "isCustomEmoji" << R"(","size":)" << json.size();
    size_t a=0,d=0,sp=0,b=0;
    for(char c : json) { 
        if(std::isalpha(c))a++; else if(std::isdigit(c))d++; 
        else if(c==' ')sp++; else if(c=='{'||c=='}')b++;
    }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << R"(,"spaces":)" << sp << R"(,"braces":)" << b;
    auto pos=json.find('"'); 
    if(pos!=std::string::npos){auto e=json.find('"',pos+1);if(e!=std::string::npos)oss<<R"(,"first_str":")"<<json.substr(pos+1,std::min(size_t(15),e-pos-1))<<R"(")";}
    oss << "}";
    return oss.str();
}

std::string formatReactionPicker(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "formatReactionPicker" << R"(","size":)" << json.size();
    size_t a=0,d=0,sp=0,b=0;
    for(char c : json) { 
        if(std::isalpha(c))a++; else if(std::isdigit(c))d++; 
        else if(c==' ')sp++; else if(c=='{'||c=='}')b++;
    }
    oss << R"(,"alpha":)" << a << R"(,"digits":)" << d << R"(,"spaces":)" << sp << R"(,"braces":)" << b;
    auto pos=json.find('"'); 
    if(pos!=std::string::npos){auto e=json.find('"',pos+1);if(e!=std::string::npos)oss<<R"(,"first_str":")"<<json.substr(pos+1,std::min(size_t(15),e-pos-1))<<R"(")";}
    oss << "}";
    return oss.str();
}

