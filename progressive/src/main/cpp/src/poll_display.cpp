#include "progressive/poll_display.hpp"
#include <sstream>
#include <algorithm>

namespace progressive {

static std::string extractField(const std::string& json, const std::string& key) {
    auto p = json.find("\"" + key + "\":\"");
    if (p == std::string::npos) return "";
    p += key.size() + 4;
    auto e = json.find('"', p);
    return e != std::string::npos ? json.substr(p, e - p) : "";
}

PollDisplayInfo parsePollForDisplay(const std::string& startJson,
                                      const std::string& responseJson,
                                      const std::string& myId) {
    PollDisplayInfo info;
    auto extractOptions = [&](const std::string& json) {
        size_t pos = 0;
        int optNum = 1;
        while (pos < json.size()) {
            auto idPos = json.find("\"id\":\"", pos);
            if (idPos == std::string::npos) break;
            idPos += 6;
            auto idEnd = json.find('"', idPos);
            if (idEnd == std::string::npos) break;
            
            PollOptionDisplay opt;
            opt.id = json.substr(idPos, idEnd - idPos);
            
            auto textPos = json.find("\"org.matrix.msc3381.poll.start\":{\"text\":\"", idEnd);
            if (textPos == std::string::npos) {
                textPos = json.find("\"text\":\"", idEnd);
                if (textPos != std::string::npos && textPos - idEnd < 200) {
                    textPos += 8;
                    auto textEnd = json.find('"', textPos);
                    if (textEnd != std::string::npos) opt.text = json.substr(textPos, textEnd - textPos);
                }
            }
            if (opt.text.empty()) opt.text = "Option " + std::to_string(optNum++);
            info.options.push_back(opt);
            pos = idEnd + 1;
        }
    };
    
    extractOptions(startJson);
    info.question = extractField(startJson, "question");
    if (info.question.empty()) {
        auto qPos = startJson.find("\"text\":\"", startJson.find("\"question\""));
        if (qPos != std::string::npos) {
            qPos += 8;
            auto qEnd = startJson.find('"', qPos);
            if (qEnd != std::string::npos) info.question = startJson.substr(qPos, qEnd - qPos);
        }
    }
    
    info.isClosed = isPollEnded(startJson);
    return info;
}

std::string formatPollResults(const PollDisplayInfo& p) {
    std::ostringstream os;
    os << "📊 " << p.question << "\n";
    for (const auto& opt : p.options) {
        os << formatPollBar(opt.text, opt.voteCount, p.totalVotes, opt.percentage) << "\n";
    }
    os << p.totalVotes << " total vote" << (p.totalVotes != 1 ? "s" : "");
    if (p.isClosed) os << " • Closed";
    return os.str();
}

std::string formatPollBar(const std::string& text, int votes, int total, double pct, int width) {
    int filled = (int)(pct * width / 100.0);
    std::ostringstream os;
    os << text << " [";
    for (int i = 0; i < width; i++) os << (i < filled ? "█" : "░");
    os << "] " << votes << " (" << (int)pct << "%)";
    return os.str();
}

std::string getPollWinner(const PollDisplayInfo& p) {
    if (p.options.empty()) return "";
    auto winner = *std::max_element(p.options.begin(), p.options.end(),
        [](auto& a, auto& b) { return a.voteCount < b.voteCount; });
    return winner.text + " won with " + std::to_string(winner.voteCount) + " votes";
}

bool isPollEnded(const std::string& json) {
    return json.find("\"m.poll.end\"") != std::string::npos ||
           json.find("\"org.matrix.msc3381.poll.end\"") != std::string::npos;
}

} // namespace progressive
