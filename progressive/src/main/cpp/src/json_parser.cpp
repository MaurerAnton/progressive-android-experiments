#include "progressive/json_parser.hpp"
#include <cstdlib>
#include <cstddef>

namespace progressive {

std::string parseJsonStringValue(const std::string& json, const std::string& key) {
    // Search for "key"
    std::string search = '"' + key + '"';
    auto pos = json.find(search);
    if (pos == std::string::npos) return {};

    pos += search.size();

    // Skip whitespace and colon
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n' || json[pos] == '\r'))
        ++pos;
    if (pos >= json.size() || json[pos] != ':') return {};
    ++pos;

    // Skip whitespace after colon
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n' || json[pos] == '\r'))
        ++pos;
    if (pos >= json.size()) return {};

    // Handle string value: "xxx"
    if (json[pos] == '"') {
        ++pos;
        auto end = json.find('"', pos);
        if (end == std::string::npos) return {};
        return json.substr(pos, end - pos);
    }

    // Handle numeric/literal value: 123, true, false, null
    auto end = pos;
    while (end < json.size() && json[end] != ',' && json[end] != '}' && json[end] != ' ' &&
           json[end] != '\t' && json[end] != '\n' && json[end] != '\r') {
        ++end;
    }
    return json.substr(pos, end - pos);
}


bool parseJsonBoolValue(const std::string& json, const std::string& key, bool defaultValue) {
    auto val = parseJsonStringValue(json, key);
    if (val.empty()) return defaultValue;
    return val == "true";
}

int64_t parseJsonInt64Value(const std::string& json, const std::string& key, int64_t defaultValue) {
    auto val = parseJsonStringValue(json, key);
    if (val.empty()) return defaultValue;
    return std::strtoll(val.c_str(), nullptr, 10);
}



// ---- Convenience helpers ----

bool parseJsonBool(const std::string& json, const std::string& key, bool def) {
    auto val = parseJsonStringValue(json, key);
    if (val.empty()) return def;
    return val == "true" || val == "1";
}

int64_t parseJsonInt64(const std::string& json, const std::string& key, int64_t def) {
    auto val = parseJsonStringValue(json, key);
    if (val.empty()) return def;
    try { return std::stoll(val); } catch(...) { return def; }
}

double parseJsonDouble(const std::string& json, const std::string& key, double def) {
    auto val = parseJsonStringValue(json, key);
    if (val.empty()) return def;
    try { return std::stod(val); } catch(...) { return def; }
}

std::vector<std::string> parseJsonStringArray(const std::string& json, const std::string& key) {
    std::vector<std::string> result;
    auto keyPos = json.find("\"" + key + "\"");
    if (keyPos == std::string::npos) return result;
    auto arrStart = json.find('[', keyPos);
    auto arrEnd = json.find(']', arrStart);
    if (arrStart == std::string::npos || arrEnd == std::string::npos) return result;
    std::string arr = json.substr(arrStart + 1, arrEnd - arrStart - 1);
    size_t pos = 0;
    while (pos < arr.size()) {
        auto q1 = arr.find('"', pos);
        if (q1 == std::string::npos) break;
        auto q2 = arr.find('"', q1 + 1);
        if (q2 == std::string::npos) break;
        result.push_back(arr.substr(q1 + 1, q2 - q1 - 1));
        pos = q2 + 1;
    }
    return result;
}

bool parseJsonHasKey(const std::string& json, const std::string& key) {
    return json.find("\"" + key + "\"") != std::string::npos;
}

std::string parseJsonObject(const std::string& json, const std::string& key) {
    auto keyPos = json.find("\"" + key + "\":");
    if (keyPos == std::string::npos) return "";
    keyPos += key.size() + 3;
    while (keyPos < json.size() && json[keyPos] == ' ') keyPos++;
    if (keyPos >= json.size() || json[keyPos] != '{') return "";
    int depth = 0;
    size_t start = keyPos;
    for (size_t i = keyPos; i < json.size(); i++) {
        if (json[i] == '{') depth++;
        else if (json[i] == '}') { depth--; if (depth == 0) { keyPos = i + 1; break; } }
    }
    return json.substr(start, keyPos - start);
}

} // namespace progressive
