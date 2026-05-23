#include "progressive/room_topic_utils.hpp"
namespace progressive {
std::string buildTopicEvent(const std::string& t) { return R"({"topic":")" + t + R"("})"; }
std::string parseTopicFromEvent(const std::string& json) {
    auto p = json.find("\"topic\":\""); if (p == std::string::npos) return "";
    p += 9; auto e = json.find('"', p); return e != std::string::npos ? json.substr(p, e - p) : "";
}
std::string truncateTopic(const std::string& t, int max) { return t.size() > (size_t)max ? t.substr(0, max-3)+"..." : t; }
std::string formatTopicPreview(const std::string& t, int max) { return truncateTopic(t, max); }
}
