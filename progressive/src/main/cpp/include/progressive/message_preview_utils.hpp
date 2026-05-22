#pragma once
#include <string>
#include <cstdint>

namespace progressive {

// Format message preview for notification/room list
std::string formatMessagePreview(const std::string& body, int maxLen = 80);

// Format sender prefix ("You: " or "Name: ")
std::string formatSenderPrefix(const std::string& senderName, bool isOwnMessage);

// Get preview icon from message type
std::string getMessagePreviewIcon(const std::string& msgType);

// Build preview text for different message types
std::string buildImagePreview(const std::string& body);
std::string buildVideoPreview(const std::string& body);
std::string buildFilePreview(const std::string& body, const std::string& filename);
std::string buildAudioPreview(const std::string& body);
std::string buildStickerPreview(const std::string& body);

// Truncate text with ellipsis
std::string truncatePreview(const std::string& text, int maxLen = 80);

} // namespace progressive
