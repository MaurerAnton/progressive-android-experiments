#pragma once
#include <string>
#include <cstdint>

namespace progressive {

struct EncryptionInfo {
    std::string algorithm;      // "m.megolm.v1.aes-sha2"
    std::string senderKey;      // curve25519 key
    std::string sessionId;      // megolm session ID
    std::string ciphertext;     // encrypted payload
    std::string deviceId;       // sender's device
    int messageIndex = 0;
    bool isValid = false;
};

// Parse m.room.encrypted event content
EncryptionInfo parseEncryptedEvent(const std::string& json);

// Check if an event is encrypted
bool isEncryptedEvent(const std::string& json);

// Build encrypted event content (for sending, placeholder)
std::string buildEncryptedContent(const std::string& ciphertext, const std::string& senderKey,
                                    const std::string& sessionId, const std::string& deviceId);

// Format encryption info for UI display
std::string formatEncryptionInfo(const EncryptionInfo& info);

// Check if encryption matches expected algorithm
bool isMegolmEncrypted(const std::string& json);

} // namespace progressive
