#include "progressive/message_encryption.hpp"
#include <sstream>

namespace progressive {

static std::string extractField(const std::string& json, const std::string& key) {
    auto p = json.find("\"" + key + "\":\"");
    if (p == std::string::npos) return "";
    p += key.size() + 4;
    auto e = json.find('"', p);
    if (e == std::string::npos) return "";
    return json.substr(p, e - p);
}

EncryptionInfo parseEncryptedEvent(const std::string& json) {
    EncryptionInfo info;
    info.algorithm = extractField(json, "algorithm");
    info.senderKey = extractField(json, "sender_key");
    info.sessionId = extractField(json, "session_id");
    info.ciphertext = extractField(json, "ciphertext");
    info.deviceId = extractField(json, "device_id");
    
    // Parse message_index from JSON number
    auto miPos = json.find("\"session_id\":");
    if (miPos != std::string::npos) {
        auto msgIdx = json.find("\"message_index\":");
        if (msgIdx != std::string::npos) {
            msgIdx += 16;
            while (msgIdx < json.size() && json[msgIdx] == ' ') msgIdx++;
            try { info.messageIndex = std::stoi(json.substr(msgIdx)); } catch(...) {}
        }
    }
    info.isValid = !info.algorithm.empty() && !info.ciphertext.empty();
    return info;
}

bool isEncryptedEvent(const std::string& json) {
    return json.find("\"algorithm\":\"m.megolm") != std::string::npos ||
           json.find("\"algorithm\": \"m.megolm") != std::string::npos;
}

bool isMegolmEncrypted(const std::string& json) {
    return json.find("megolm") != std::string::npos;
}

std::string buildEncryptedContent(const std::string& ciphertext, const std::string& senderKey,
                                    const std::string& sessionId, const std::string& deviceId) {
    std::ostringstream os;
    os << "{";
    os << R"("algorithm":"m.megolm.v1.aes-sha2",)";
    os << R"("sender_key":")" << senderKey << R"(",)";
    os << R"("session_id":")" << sessionId << R"(",)";
    os << R"("device_id":")" << deviceId << R"(",)";
    os << R"("ciphertext":")" << ciphertext << R"(")";
    os << "}";
    return os.str();
}

std::string formatEncryptionInfo(const EncryptionInfo& info) {
    if (!info.isValid) return "Unknown encryption";
    std::ostringstream os;
    os << "Encrypted (" << info.algorithm << ")";
    if (!info.deviceId.empty()) os << " from " << info.deviceId;
    return os.str();
}

} // namespace progressive
