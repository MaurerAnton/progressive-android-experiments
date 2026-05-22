#pragma once
#include <string>

namespace progressive {

// Build reply formatted body (HTML with <mx-reply>)
std::string buildReplyFormattedBody(const std::string& repliedBody, const std::string& repliedFormatted,
                                      const std::string& repliedSender, const std::string& repliedUserId,
                                      const std::string& newBody, const std::string& newFormatted);

// Build reply plain text body
std::string buildReplyPlainBody(const std::string& repliedBody, const std::string& newBody);

// Extract replied event ID from reply event
std::string extractRepliedEventId(const std::string& json);

// Check if event is a reply
bool isReplyEvent(const std::string& json);

} // namespace progressive
