#include "progressive/account_export.hpp"
#include "progressive/hash_utils.hpp"
#include <sstream>
#include <cstring>
#include <vector>

namespace progressive {

std::string encryptAccountData(const AccountData& data, const std::string& passphrase) {
    auto json = accountToJson(data);
    // XOR with passphrase-derived key
    for (size_t i = 0; i < json.size(); ++i) {
        json[i] ^= passphrase[i % passphrase.size()];
    }
    auto bytes = std::vector<uint8_t>(json.begin(), json.end());
    return progressive::base64Encode(bytes);
}

AccountData decryptAccountData(const std::string& encrypted, const std::string& passphrase) {
    auto decodedBytes = progressive::base64Decode(encrypted);
    std::string decoded(decodedBytes.begin(), decodedBytes.end());
    if (decoded.empty()) return {};

    for (size_t i = 0; i < decoded.size(); ++i) {
        decoded[i] ^= passphrase[i % passphrase.size()];
    }

    auto data = jsonToAccount(decoded);
    // Basic validation: must have userId and accessToken
    if (data.userId.empty() || data.accessToken.empty() || data.homeServerUrl.empty()) {
        return {};
    }
    return data;
}

} // namespace progressive
