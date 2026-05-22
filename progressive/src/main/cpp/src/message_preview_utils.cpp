#include "progressive/message_preview_utils.hpp"
#include <algorithm>

namespace progressive {

std::string truncatePreview(const std::string& text, int maxLen) {
    if ((int)text.size() <= maxLen) return text;
    return text.substr(0, maxLen - 3) + "...";
}
std::string formatMessagePreview(const std::string& body, int maxLen) {
    std::string result;
    bool inTag = false;
    for (char c : body) { if (c == '<') inTag = true; else if (c == '>') inTag = false; else if (!inTag) result += c; }
    return truncatePreview(result, maxLen);
}
std::string formatSenderPrefix(const std::string& name, bool own) {
    return own ? "You: " : name + ": ";
}
std::string getMessagePreviewIcon(const std::string& type) {
    if (type == "m.image" || type == "image") return "🖼️";
    if (type == "m.video" || type == "video") return "🎬";
    if (type == "m.audio" || type == "audio") return "🎵";
    if (type == "m.file" || type == "file") return "📎";
    if (type == "m.sticker") return "🏷️"; return "💬";
}
std::string buildImagePreview(const std::string& b) { return "🖼️ " + (b.empty() ? "Image" : b); }
std::string buildVideoPreview(const std::string& b) { return "🎬 " + (b.empty() ? "Video" : b); }
std::string buildFilePreview(const std::string& b, const std::string& fn) { return "📎 " + (fn.empty() ? (b.empty() ? "File" : b) : fn); }
std::string buildAudioPreview(const std::string& b) { return "🎵 " + (b.empty() ? "Voice message" : b); }
std::string buildStickerPreview(const std::string& b) { return "🏷️ " + (b.empty() ? "Sticker" : b); }

} // namespace progressive
