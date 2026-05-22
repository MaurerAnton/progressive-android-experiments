#include "progressive/content_builder_utils.hpp"
#include <sstream>

namespace progressive {

std::string buildTextContent(const std::string& body, const std::string& formattedBody) {
    std::ostringstream os;
    os << R"({"msgtype":"m.text","body":")" << body << R"(")";
    if (!formattedBody.empty()) os << R"(,"formatted_body":")" << formattedBody << R"(")";
    if (!formattedBody.empty()) os << R"(,"format":"org.matrix.custom.html")";
    os << "}";
    return os.str();
}

std::string buildNoticeContent(const std::string& body) {
    return R"({"msgtype":"m.notice","body":")" + body + R"("})";
}

std::string buildEmoteContent(const std::string& body) {
    return R"({"msgtype":"m.emote","body":")" + body + R"("})";
}

std::string buildImageContent(const std::string& url, const std::string& body,
                                int w, int h, int64_t size, const std::string& mime) {
    std::ostringstream os;
    os << R"({"msgtype":"m.image","body":")" << body << R"(")";
    os << R"(,"url":")" << url << R"(")";
    os << R"(,"info":{"mimetype":")" << mime << R"(")";
    os << R"(,"w":)" << w << R"(,"h":)" << h << R"(,"size":)" << size << "}";
    os << "}";
    return os.str();
}

std::string buildVideoContent(const std::string& url, const std::string& body,
                                int w, int h, int dur, const std::string& mime) {
    std::ostringstream os;
    os << R"({"msgtype":"m.video","body":")" << body << R"(")";
    os << R"(,"url":")" << url << R"(")";
    os << R"(,"info":{"mimetype":")" << mime << R"(")";
    os << R"(,"w":)" << w << R"(,"h":)" << h << R"(,"duration":)" << dur << "}";
    os << "}";
    return os.str();
}

std::string buildStickerContent(const std::string& url, const std::string& body) {
    return R"({"msgtype":"m.sticker","body":")" + body + R"(","url":")" + url + R"("})";
}

std::string buildTopicChangeContent(const std::string& topic) {
    return R"({"topic":")" + topic + R"("})";
}

std::string buildRoomNameChangeContent(const std::string& name) {
    return R"({"name":")" + name + R"("})";
}

std::string buildContentJson(const std::string& msgType, const std::string& body,
                              const std::string& extra) {
    std::ostringstream os;
    os << R"({"msgtype":")" << msgType << R"(","body":")" << body << R"(")";
    if (!extra.empty()) os << "," << extra;
    os << "}";
    return os.str();
}

} // namespace progressive
