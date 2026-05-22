#include "progressive/audio_engine.hpp"
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace progressive {

std::string formatPositionInfo(int64_t positionMs, int64_t durationMs) {
    return formatDuration(positionMs) + " / " + formatDuration(durationMs);
}

float computeProgress(int64_t positionMs, int64_t durationMs) {
    if (durationMs <= 0) return 0.0f;
    float p = static_cast<float>(positionMs) / static_cast<float>(durationMs);
    if (p < 0.0f) p = 0.0f;
    if (p > 1.0f) p = 1.0f;
    return p;
}

bool isSupportedAudioType(const std::string& mimeType) {
    // Audio formats supported by Android MediaPlayer
    static const char* supported[] = {
        "audio/mpeg", "audio/mp3", "audio/mp4", "audio/m4a",
        "audio/ogg", "audio/opus", "audio/wav", "audio/wave",
        "audio/x-wav", "audio/flac", "audio/aac", "audio/webm",
        "audio/3gpp", "audio/x-ms-wma", "audio/amr"
    };
    for (const auto& s : supported) {
        if (mimeType == s) return true;
    }
    return false;
}

std::string mimeToExtension(const std::string& mimeType) {
    if (mimeType == "audio/mpeg" || mimeType == "audio/mp3") return "mp3";
    if (mimeType == "audio/mp4" || mimeType == "audio/m4a") return "m4a";
    if (mimeType == "audio/ogg") return "ogg";
    if (mimeType == "audio/opus") return "opus";
    if (mimeType == "audio/wav" || mimeType == "audio/wave" || mimeType == "audio/x-wav") return "wav";
    if (mimeType == "audio/flac") return "flac";
    if (mimeType == "audio/aac") return "aac";
    if (mimeType == "audio/webm") return "webm";
    if (mimeType == "audio/3gpp") return "3gp";
    if (mimeType == "audio/amr") return "amr";
    return "";
}



// ---- Audio helpers ----

bool isAudioMessage(const std::string& json) {
    return json.find(""msgtype":"m.audio"") != std::string::npos;
}

int parseAudioDuration(const std::string& json) {
    auto durPos = json.find(""duration":");
    if (durPos == std::string::npos) return 0;
    durPos += 11;
    try { return std::stoi(json.substr(durPos)); } catch(...) { return 0; }
}

std::string formatAudioDurationDisplay(int ms) {
    int totalSec = ms / 1000;
    int min = totalSec / 60;
    int sec = totalSec % 60;
    std::ostringstream os;
    os << min << ":";
    if (sec < 10) os << "0";
    os << sec;
    return os.str();
}

bool isAudioPlaying(const std::string& mxcUrl, const std::string& currentPlayingUrl) {
    return mxcUrl == currentPlayingUrl;
}

std::string buildAudioPlayState(const std::string& mxcUrl, bool playing, int positionMs) {
    std::ostringstream os;
    os << R"({"url":")" << mxcUrl << R"(")";
    os << R"(,"playing":)" << (playing ? "true" : "false");
    os << R"(,"position":)" << positionMs;
    os << "}";
    return os.str();
}

} // namespace progressive
