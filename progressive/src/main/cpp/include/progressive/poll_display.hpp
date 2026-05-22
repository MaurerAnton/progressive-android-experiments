#pragma once
#include <string>
#include <vector>

namespace progressive {

struct PollOptionDisplay {
    std::string id;             // "option_1"
    std::string text;
    int voteCount = 0;
    double percentage = 0.0;    // 0.0-100.0
    bool isWinner = false;
    bool isSelected = false;    // current user voted for this
};

struct PollDisplayInfo {
    std::string pollId;
    std::string question;
    std::vector<PollOptionDisplay> options;
    int totalVotes = 0;
    int maxSelections = 1;
    bool isClosed = false;
    bool isDisclosed = true;    // can see who voted
    std::string winnerText;     // "Option A won"
};

// Parse poll start event for display
PollDisplayInfo parsePollForDisplay(const std::string& pollStartJson,
                                      const std::string& pollResponseJson = "",
                                      const std::string& myUserId = "");

// Format poll results for timeline
std::string formatPollResults(const PollDisplayInfo& poll);

// Format poll progress bar (text-based)
std::string formatPollBar(const std::string& text, int votes, int total, double pct, int barWidth = 20);

// Get poll winner
std::string getPollWinner(const PollDisplayInfo& poll);

// Check if poll has ended
bool isPollEnded(const std::string& pollEndJson);

} // namespace progressive
