// Auto-generated stubs with exact return types from headers
#include "progressive/content_utils.hpp"
#include "progressive/cross_signing_manager.hpp"
#include "progressive/device_manager_full.hpp"
#include "progressive/poll_manager.hpp"
#include "progressive/room_directory_manager.hpp"
#include "progressive/room_state_manager.hpp"
#include "progressive/server_notice_manager.hpp"
#include "progressive/space_graph.hpp"

namespace progressive {



std::string buildMxcUri(const std::string& serverName, const std::string& mediaId) {
    return "mxc://" + serverName + "/" + mediaId;
}
std::string ensureCorrectFormattedBodyInTextReply(
    const std::string& newFormattedBody,
    const std::string& newBody,
    const std::string& originalFormattedBody) {

    const char* MX_REPLY_END_TAG = "</mx-reply>";

    // Only fix if new formatted_body is missing the reply end tag
    // while the original had one
    if (newFormattedBody.empty()) return newBody;
    if (newFormattedBody.find(MX_REPLY_END_TAG) != std::string::npos) return newFormattedBody;
    if (originalFormattedBody.find(MX_REPLY_END_TAG) == std::string::npos) return newFormattedBody;

    // Merge: take original's <mx-reply>... wrapper + new body
    auto endPos = originalFormattedBody.rfind(MX_REPLY_END_TAG);
    if (endPos == std::string::npos) return newFormattedBody;
    endPos += strlen(MX_REPLY_END_TAG);

    return originalFormattedBody.substr(0, endPos) + newBody;
}
std::string extractUsefulTextFromReply(const std::string& repliedBody) {
    if (repliedBody.empty()) return "";

    // Split into lines
    std::vector<std::string> lines;
    std::string current;
    for (char c : repliedBody) {
        if (c == '\n') { lines.push_back(current); current.clear(); }
        else current += c;
    }
    if (!current.empty()) lines.push_back(current);

    // Original: wellFormed = repliedBody.startsWith(">")
    bool wellFormed = !lines.empty() && !lines[0].empty() && lines[0][0] == '>';
    bool endOfPreviousFound = false;
    std::string usefulText;

    for (const auto& line : lines) {
        // Original: if (it == "") { endOfPreviousFound = true; return@forEach }
        if (line.empty()) {
            endOfPreviousFound = true;
            continue;
        }
        if (!endOfPreviousFound) {
            // Original: wellFormed = wellFormed && it.startsWith(">")
            wellFormed = wellFormed && !line.empty() && line[0] == '>';
        } else {
            if (!usefulText.empty()) usefulText += '\n';
            usefulText += line;
        }
    }

    // Original: return usefulLines.joinToString("\n").takeIf { wellFormed } ?: repliedBody
    return wellFormed ? usefulText : repliedBody;
}
std::string formatSpoilerTextFromHtml(const std::string& formattedBody) {
    // Original: replaces <span data-mx-spoiler>content</span> with spoiler chars
    // Replaces content between <span data-mx-spoiler> and </span> with block chars
    std::string result;
    const std::string SPOILER_OPEN = "<span data-mx-spoiler>";
    const std::string SPOILER_CLOSE = "</span>";
    const char SPOILER_CHAR = '\xDB'; // █

    size_t pos = 0;
    while (pos < formattedBody.size()) {
        auto openPos = formattedBody.find(SPOILER_OPEN, pos);
        if (openPos == std::string::npos) {
            result += formattedBody.substr(pos);
            break;
        }

        result += formattedBody.substr(pos, openPos - pos);
        openPos += SPOILER_OPEN.size();

        auto closePos = formattedBody.find(SPOILER_CLOSE, openPos);
        if (closePos == std::string::npos) {
            result += formattedBody.substr(openPos - SPOILER_OPEN.size());
            pos = openPos;
            continue;
        }

        // Count visible characters between tags
        std::string content = formattedBody.substr(openPos, closePos - openPos);
        size_t visibleChars = 0;
        bool inTag = false;
        for (char c : content) { if (c == '<') inTag = true; else if (c == '>') inTag = false; else if (!inTag) visibleChars++; }

        // Replace with spoiler chars
        result += std::string(visibleChars, SPOILER_CHAR);
        pos = closePos + SPOILER_CLOSE.size();
    }

    return result;
}
std::string getEditedTargetEventId(const std::string& contentJson) {
    // Original: getRelationContent()?.takeIf { it.type == REPLACE }?.eventId
    auto relatesPos = contentJson.find("\"m.relates_to\"");
    if (relatesPos == std::string::npos) return "";

    // Find rel_type: "m.replace"
    auto typePos = contentJson.find("\"rel_type\":\"m.replace\"", relatesPos);
    if (typePos == std::string::npos) {
        typePos = contentJson.find("\"rel_type\": \"m.replace\"", relatesPos);
    }
    if (typePos == std::string::npos || typePos > contentJson.find('}
std::string getExtensionFromMimeType(const std::string& mimetype) {
    if (mimetype == "image/jpeg") return ".jpg";
    if (mimetype == "image/png") return ".png";
    if (mimetype == "image/gif") return ".gif";
    if (mimetype == "image/webp") return ".webp";
    if (mimetype == "image/svg+xml") return ".svg";
    if (mimetype == "video/mp4") return ".mp4";
    if (mimetype == "video/webm") return ".webm";
    if (mimetype == "audio/mpeg") return ".mp3";
    if (mimetype == "audio/ogg") return ".ogg";
    if (mimetype == "audio/wav") return ".wav";
    if (mimetype == "application/pdf") return ".pdf";
    if (mimetype == "application/zip") return ".zip";
    if (mimetype == "text/plain") return ".txt";
    return "";
}
std::string getLatestEditEventId(const std::string& editSummaryJson, const std::string& originalEventId) {
    // Original: annotations?.editSummary?.sourceEvents?.lastOrNull() ?: eventId
    if (editSummaryJson.empty()) return originalEventId;

    // Find last event_id in sourceEvents array
    auto eventsPos = editSummaryJson.find("\"sourceEvents\"");
    if (eventsPos == std::string::npos) return originalEventId;

    // Find last quoted event_id
    auto lastQuote = editSummaryJson.rfind("\"event_id\":\"", editSummaryJson.find(']', eventsPos));
    if (lastQuote == std::string::npos) {
        lastQuote = editSummaryJson.rfind("\"event_id\": \"", editSummaryJson.find(']', eventsPos));
    }
    if (lastQuote == std::string::npos || lastQuote < eventsPos) return originalEventId;

    lastQuote += 12; // skip "event_id":"
    auto end = editSummaryJson.find('"', lastQuote);
    return (end != std::string::npos) ? editSummaryJson.substr(lastQuote, end - lastQuote) : originalEventId;
}
bool hasTextWithImage(const std::string& contentJson) {
    return contentJson.find("\"format\"") != std::string::npos &&
           contentJson.find("\"formatted_body\"") != std::string::npos &&
           contentJson.find("<img") != std::string::npos;
}
std::string normalizeMimeType(const std::string& mimeType) {
    // "image/jpg" → "image/jpeg"
    if (mimeType == "image/jpg") return "image/jpeg";
    return mimeType;
}

// Constructors for missing modules
std::string resolveMxcThumbnailUrl(const std::string& mxcUrl, const std::string& homeServerUrl,
    int width, int height, const std::string& method) {
    if (!isMxcUri(mxcUrl)) return mxcUrl;

    auto server = extractMxcServerName(mxcUrl);
    auto mediaId = extractMxcMediaId(mxcUrl);
    if (server.empty() || mediaId.empty()) return mxcUrl;

    std::string base = homeServerUrl;
    while (!base.empty() && base.back() == '/') base.pop_back();

    // Matrix spec: GET /_matrix/media/v3/thumbnail/{serverName}/{mediaId}?w=...&h=...&method=...
    std::ostringstream out;
    out << base << "/_matrix/media/v3/thumbnail/" << server << "/" << mediaId;
    out << "?width=" << width << "&height=" << height << "&method=" << method;
    return out.str();
}
} 
