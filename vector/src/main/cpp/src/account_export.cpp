     1|#include "progressive/account_export.hpp"
     2|#include <sstream>
     3|#include <cstring>
     4|#include <vector>
     5|
     6|namespace progressive {
     7|
     8|static const char BASE64_CHARS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
     9|
    10|std::string base64Encode(const std::string& input) {
    11|    std::string out;
    12|    out.reserve((input.size() + 2) / 3 * 4);
    13|    int val = 0, bits = -6;
    14|    for (unsigned char c : std::vector<unsigned char>(input.begin(), input.end())) {
    15|        val = (val << 8) + c;
    16|        bits += 8;
    17|        while (bits >= 0) {
    18|            out += BASE64_CHARS[(val >> bits) & 0x3F];
    19|            bits -= 6;
    20|        }
    21|    }
    22|    if (bits > -6) out += BASE64_CHARS[((val << 8) >> (bits + 8)) & 0x3F];
    23|    while (out.size() % 4) out += '=';
    24|    return out;
    25|}
    26|
    27|std::string base64Decode(const std::string& input) {
    28|    std::vector<int> T(256, -1);
    29|    for (int i = 0; i < 64; i++) T[BASE64_CHARS[i]] = i;
    30|    std::string out;
    31|    out.reserve(input.size() * 3 / 4);
    32|    int val = 0, bits = -8;
    33|    for (unsigned char c : input) {
    34|        if (T[c] == -1) break;
    35|        val = (val << 6) + T[c];
    36|        bits += 6;
    37|        if (bits >= 0) {
    38|            out += char((val >> bits) & 0xFF);
    39|            bits -= 8;
    40|        }
    41|    }
    42|    return out;
    43|}
    44|
    45|std::string accountToJson(const AccountData& data) {
    46|    auto esc = [](const std::string& s) -> std::string {
    47|        std::string out;
    48|        for (char c : s) {
    49|            if (c == '"') out += "\\\"";
    50|            else if (c == '\\') out += "\\\\";
    51|            else if (c == '\n') out += "\\n";
    52|            else out += c;
    53|        }
    54|        return out;
    55|    };
    56|
    57|    std::ostringstream json;
    58|    json << "{";
    59|    json << R"("userId": ")" << esc(data.userId) << R"(",)";
    60|    json << R"("accessToken": ")" << esc(data.accessToken) << R"(",)";
    61|    json << R"("refreshToken": ")" << esc(data.refreshToken) << R"(",)";
    62|    json << R"("homeServerUrl": ")" << esc(data.homeServerUrl) << R"(",)";
    63|    json << R"("deviceId": ")" << esc(data.deviceId) << R"(",)";
    64|    json << R"("deviceName": ")" << esc(data.deviceName) << R"(",)";
    65|    json << R"("displayName": ")" << esc(data.displayName) << R"(",)";
    66|    json << R"("avatarUrl": ")" << esc(data.avatarUrl) << R"(",)";
    67|    json << R"("includeCache": )" << (data.includeCache ? "true" : "false");
    68|    json << "}";
    69|    return json.str();
    70|}
    71|
    72|AccountData jsonToAccount(const std::string& json) {
    73|    AccountData data;
    74|    auto getStr = [&](const std::string& key) -> std::string {
    75|        auto search = '"' + key + '"';
    76|        auto pos = json.find(search);
    77|        if (pos == std::string::npos) return {};
    78|        pos += search.size();
    79|        while (pos < json.size() && (json[pos] == ' ' || json[pos] == ':' || json[pos] == '\t')) ++pos;
    80|        if (pos >= json.size()) return {};
    81|        // Expect string value
    82|        if (json[pos] == '"') {
    83|            ++pos;
    84|            auto end = json.find('"', pos);
    85|            if (end == std::string::npos) return {};
    86|            return json.substr(pos, end - pos);
    87|        }
    88|        // Boolean or number
    89|        auto end = pos;
    90|        while (end < json.size() && json[end] != ',' && json[end] != '}' && json[end] != ' ') ++end;
    91|        return json.substr(pos, end - pos);
    92|    };
    93|
    94|    data.userId       = getStr("userId");
    95|    data.accessToken  = getStr("accessToken");
    96|    data.refreshToken = getStr("refreshToken");
    97|    data.homeServerUrl = getStr("homeServerUrl");
    98|    data.deviceId     = getStr("deviceId");
    99|    data.deviceName   = getStr("deviceName");
   100|    data.displayName  = getStr("displayName");
   101|    data.avatarUrl    = getStr("avatarUrl");
   102|    data.includeCache = getStr("includeCache") == "true";
   103|    return data;
   104|}
   105|
   106|std::string encryptAccountData(const AccountData& data, const std::string& passphrase) {
   107|    auto json = accountToJson(data);
   108|    // XOR with passphrase-derived key
   109|    for (size_t i = 0; i < json.size(); ++i) {
   110|        json[i] ^= passphrase[i % passphrase.size()];
   111|    }
   112|    return base64Encode(json);
   113|}
   114|
   115|AccountData decryptAccountData(const std::string& encrypted, const std::string& passphrase) {
   116|    auto decoded = base64Decode(encrypted);
   117|    if (decoded.empty()) return {};
   118|
   119|    for (size_t i = 0; i < decoded.size(); ++i) {
   120|        decoded[i] ^= passphrase[i % passphrase.size()];
   121|    }
   122|
   123|    auto data = jsonToAccount(decoded);
   124|    // Basic validation: must have userId and accessToken
   125|    if (data.userId.empty() || data.accessToken.empty() || data.homeServerUrl.empty()) {
   126|        return {};
   127|    }
   128|    return data;
   129|}
   130|
   131|} // namespace progressive
   132|