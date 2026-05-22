#include "progressive/crypto_device_utils.hpp"
#include <sstream>
#include <algorithm>

namespace progressive {

CryptoDeviceInfo parseCryptoDeviceInfo(const std::string& deviceId, const std::string& userId,
                                         const std::string& json) {
    CryptoDeviceInfo d;
    d.deviceId = deviceId;
    d.userId = userId;
    
    auto extract = [&](const std::string& key) -> std::string {
        auto p = json.find("\"" + key + "\":\"");
        if (p == std::string::npos) return "";
        p += key.size() + 4;
        auto e = json.find('"', p);
        if (e == std::string::npos) return "";
        return json.substr(p, e - p);
    };
    
    d.ed25519Key = extract("ed25519:" + deviceId);
    d.curve25519Key = extract("curve25519:" + deviceId);
    d.isVerified = json.find("\"verified\":true") != std::string::npos;
    d.isBlocked = json.find("\"blocked\":true") != std::string::npos;
    
    // Parse algorithms array
    auto algoPos = json.find("\"algorithms\"");
    if (algoPos != std::string::npos) {
        auto arr = json.find('[', algoPos);
        auto arrEnd = json.find(']', arr);
        if (arr != std::string::npos && arrEnd != std::string::npos) {
            std::string arrStr = json.substr(arr + 1, arrEnd - arr - 1);
            size_t pos = 0;
            while (pos < arrStr.size()) {
                auto q1 = arrStr.find('"', pos);
                if (q1 == std::string::npos) break;
                auto q2 = arrStr.find('"', q1 + 1);
                if (q2 == std::string::npos) break;
                d.algorithms.push_back(arrStr.substr(q1 + 1, q2 - q1 - 1));
                pos = q2 + 1;
            }
        }
    }
    
    return d;
}

std::string formatDeviceKey(const std::string& key, int truncateLen) {
    if (key.empty()) return "No key";
    if ((int)key.size() <= truncateLen) return key;
    return key.substr(0, truncateLen) + "...";
}

bool deviceSupportsAlgorithm(const CryptoDeviceInfo& d, const std::string& algo) {
    return std::find(d.algorithms.begin(), d.algorithms.end(), algo) != d.algorithms.end();
}

bool isActiveDevice(const CryptoDeviceInfo& d, int64_t) { return d.isActive; }

std::string formatDeviceInfo(const CryptoDeviceInfo& d) {
    std::ostringstream os;
    os << "Device " << d.deviceId;
    if (d.isVerified) os << " ✅";
    if (d.isBlocked) os << " 🚫";
    os << " [" << d.algorithms.size() << " algos]";
    return os.str();

std::string getDeviceDisplayName(const CryptoDeviceInfo& d) {
    return d.deviceName.empty() ? d.deviceId : d.deviceName;
}
bool isCurrentDevice(const CryptoDeviceInfo& d, const std::string& myDeviceId) {
    return d.deviceId == myDeviceId;
}
std::string buildDeviceVerificationContent(const std::string& deviceId, const std::string& method) {
    std::ostringstream os;
    os << R"({"from_device":")" << deviceId << R"(")";
    os << R"(,"methods":[")" << method << R"("])";
    os << R"(,"timestamp":)" << std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    os << "}";
    return os.str();
}
}

} // namespace progressive
