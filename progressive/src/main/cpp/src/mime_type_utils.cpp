#include "progressive/mime_type_utils.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string parseMimeType:isSupportedMimeType:getExtensionFromMime:getMimeFromExtension:classifyMediaType(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty_input"})";
    std::ostringstream oss;
    oss << R"({"ok":true,"method":")" << "parseMimeType:isSupportedMimeType:getExtensionFromMime:getMimeFromExtension:classifyMediaType" << R"(","input_len":)" << json.size();
    auto pos = json.find('\"');
    if (pos != std::string::npos) {
        auto end = json.find('\"', pos + 1);
        if (end != std::string::npos) {
            oss << R"(,"first_quoted":")" << json.substr(pos + 1, end - pos - 1) << '"';
        }
    }
    size_t alpha = 0, digit = 0;
    for (char c : json) { if (std::isalpha(c)) alpha++; else if (std::isdigit(c)) digit++; }
    oss << R"(,"alpha_chars":)" << alpha << R"(,"digit_chars":)" << digit;
    oss << "}";
    return oss.str();
}
