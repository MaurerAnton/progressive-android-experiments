     1|#include "progressive/exporter.hpp"
     2|#include <sstream>
     3|#include <ctime>
     4|#include <cstring>
     5|
     6|namespace progressive {
     7|
     8|std::string escapeHtml(const std::string& input) {
     9|    std::string out;
    10|    out.reserve(input.size());
    11|    for (char c : input) {
    12|        switch (c) {
    13|            case '&':  out += "&amp;"; break;
    14|            case '<':  out += "&lt;"; break;
    15|            case '>':  out += "&gt;"; break;
    16|            case '"':  out += "&quot;"; break;
    17|            case '\'': out += "&#39;"; break;
    18|            default:   out += c;
    19|        }
    20|    }
    21|    return out;
    22|}
    23|
    24|std::string escapeJson(const std::string& input) {
    25|    std::string out;
    26|    out.reserve(input.size());
    27|    for (char c : input) {
    28|        switch (c) {
    29|            case '"':  out += "\\\""; break;
    30|            case '\\': out += "\\\\"; break;
    31|            case '\n': out += "\\n"; break;
    32|            case '\r': out += "\\r"; break;
    33|            case '\t': out += "\\t"; break;
    34|            default:   out += c;
    35|        }
    36|    }
    37|    return out;
    38|}
    39|
    40|std::string formatEventHtml(const ExportEvent& event, bool isContinuation) {
    41|    std::ostringstream html;
    42|
    43|    if (!isContinuation) {
    44|        html << "<div class=\"mx_EventTile\">\n";
    45|        html << "  <div class=\"mx_EventTile_info\">\n";
    46|        html << "    <span class=\"mx_EventTile_sender\">" << escapeHtml(event.senderName) << "</span>\n";
    47|        if (!event.timestamp.empty()) {
    48|            html << "    <span class=\"mx_MessageTimestamp\">" << escapeHtml(event.timestamp) << "</span>\n";
    49|        }
    50|        html << "  </div>\n";
    51|    } else {
    52|        html << "<div class=\"mx_EventTile mx_EventTile_continuation\">\n";
    53|    }
    54|
    55|    html << "  <div class=\"mx_EventTile_body\">\n";
    56|
    57|    if (event.msgType == "m.image" || event.msgType == "m.video" || event.msgType == "m.file" || event.msgType == "m.audio") {
    58|        html << "    <div class=\"mx_EventTile_attachment\">\n";
    59|        html << "      <span class=\"mx_Attachment_name\">" << escapeHtml(event.fileName.empty() ? event.msgType.substr(2) : event.fileName) << "</span>\n";
    60|        if (event.mediaSize > 0) {
    61|            html << "      <span class=\"mx_Attachment_size\">" << std::to_string(event.mediaSize) << " bytes</span>\n";
    62|        }
    63|        html << "    </div>\n";
    64|    }
    65|
    66|    if (!event.body.empty()) {
    67|        html << "    <div class=\"mx_EventTile_content\">\n";
    68|        html << "      " << escapeHtml(event.body) << "\n";
    69|        html << "    </div>\n";
    70|    }
    71|
    72|    if (event.relationType == "m.annotation" && !event.sourceEventId.empty()) {
    73|        html << "    <div class=\"mx_EventTile_reaction\">\n";
    74|        html << "      " << escapeHtml(event.body) << " (reaction to " << escapeHtml(event.sourceEventId) << ")\n";
    75|        html << "    </div>\n";
    76|    }
    77|
    78|    html << "  </div>\n";
    79|    html << "</div>\n";
    80|
    81|    return html.str();
    82|}
    83|
    84|std::string formatEventPlainText(const ExportEvent& event) {
    85|    std::ostringstream text;
    86|
    87|    if (!event.timestamp.empty()) {
    88|        text << event.timestamp << " - ";
    89|    }
    90|    text << event.senderName << ": ";
    91|
    92|    if (event.msgType == "m.image" || event.msgType == "m.video" || event.msgType == "m.file" || event.msgType == "m.audio") {
    93|        text << "[" << event.msgType.substr(2) << " attached";
    94|        if (!event.fileName.empty()) text << ": " << event.fileName;
    95|        text << "]";
    96|    }
    97|
    98|    if (!event.body.empty()) {
    99|        text << " " << event.body;
   100|    }
   101|
   102|    if (event.relationType == "m.reference" && !event.sourceEventId.empty()) {
   103|        text << " (in reply to " << event.sourceEventId << ")";
   104|    }
   105|
   106|    text << "\n";
   107|    return text.str();
   108|}
   109|
   110|std::string formatEventJson(const ExportEvent& event) {
   111|    std::ostringstream json;
   112|
   113|    json << "  {\n";
   114|    json << "    \"eventId\": \"" << escapeJson(event.eventId) << "\",\n";
   115|    json << "    \"senderId\": \"" << escapeJson(event.senderId) << "\",\n";
   116|    json << "    \"senderName\": \"" << escapeJson(event.senderName) << "\",\n";
   117|    json << "    \"timestamp\": \"" << escapeJson(event.timestamp) << "\",\n";
   118|    if (!event.eventType.empty())
   119|        json << "    \"eventType\": \"" << escapeJson(event.eventType) << "\",\n";
   120|    if (!event.msgType.empty())
   121|        json << "    \"msgType\": \"" << escapeJson(event.msgType) << "\",\n";
   122|    json << "    \"body\": \"" << escapeJson(event.body) << "\",\n";
   123|    if (!event.formattedBody.empty())
   124|        json << "    \"formattedBody\": \"" << escapeJson(event.formattedBody) << "\",\n";
   125|    if (!event.relationType.empty())
   126|        json << "    \"relationType\": \"" << escapeJson(event.relationType) << "\",\n";
   127|    if (!event.sourceEventId.empty())
   128|        json << "    \"sourceEventId\": \"" << escapeJson(event.sourceEventId) << "\",\n";
   129|    if (!event.mediaUrl.empty())
   130|        json << "    \"mediaUrl\": \"" << escapeJson(event.mediaUrl) << "\",\n";
   131|    json << "    \"isEncrypted\": " << (event.isEncrypted ? "true" : "false");
   132|    if (event.mediaSize > 0) {
   133|        json << ",\n    \"mediaSize\": " << event.mediaSize;
   134|    }
   135|    json << "\n  }";
   136|
   137|    return json.str();
   138|}
   139|
   140|std::string buildHtmlDocument(
   141|    const std::string& roomName,
   142|    const std::string& roomTopic,
   143|    const std::string& exportDate,
   144|    const std::vector<ExportEvent>& events
   145|) {
   146|    std::ostringstream html;
   147|
   148|    html << "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n";
   149|    html << "<meta charset=\"UTF-8\">\n";
   150|    html << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
   151|    html << "<title>" << escapeHtml(roomName) << " — Chat Export</title>\n";
   152|    html << R"(<style>
   153|body { font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif; margin: 0; padding: 16px; background: #f5f5f5; }
   154|.mx_ExportHeader { background: #fff; border-radius: 8px; padding: 16px; margin-bottom: 16px; box-shadow: 0 1px 3px rgba(0,0,0,0.1); }
   155|.mx_ExportHeader h1 { margin: 0 0 8px; font-size: 1.5em; }
   156|.mx_ExportHeader p { margin: 4px 0; color: #666; font-size: 0.9em; }
   157|.mx_EventTile { background: #fff; border-radius: 8px; padding: 12px 16px; margin-bottom: 8px; box-shadow: 0 1px 2px rgba(0,0,0,0.05); }
   158|.mx_EventTile_continuation { margin-top: -4px; border-radius: 0 0 8px 8px; }
   159|.mx_EventTile_info { margin-bottom: 6px; }
   160|.mx_EventTile_sender { font-weight: 600; color: #333; margin-right: 8px; }
   161|.mx_MessageTimestamp { color: #999; font-size: 0.85em; }
   162|.mx_EventTile_body { color: #222; line-height: 1.5; }
   163|.mx_EventTile_attachment { background: #f0f0f0; border-radius: 4px; padding: 8px; margin-bottom: 8px; }
   164|.mx_Attachment_name { font-weight: 500; }
   165|.mx_Attachment_size { color: #999; margin-left: 8px; }
   166|.mx_EventTile_reaction { font-style: italic; color: #666; }
   167|.mx_EventTile_content { white-space: pre-wrap; word-wrap: break-word; }
   168|hr { border: none; border-top: 1px solid #e0e0e0; margin: 16px 0; }
   169|</style>)\n";
   170|    html << "</head>\n<body>\n";
   171|
   172|    html << "<div class=\"mx_ExportHeader\">\n";
   173|    html << "  <h1>" << escapeHtml(roomName) << "</h1>\n";
   174|    if (!roomTopic.empty()) {
   175|        html << "  <p>" << escapeHtml(roomTopic) << "</p>\n";
   176|    }
   177|    html << "  <p>Exported: " << exportDate << "</p>\n";
   178|    html << "  <p>Total messages: " << events.size() << "</p>\n";
   179|    html << "</div>\n";
   180|
   181|    if (events.empty()) {
   182|        html << "<p>No messages to export.</p>\n";
   183|    }
   184|
   185|    std::string prevSender;
   186|    for (const auto& event : events) {
   187|        bool isContinuation = (event.senderId == prevSender);
   188|        html << formatEventHtml(event, isContinuation);
   189|        prevSender = event.senderId;
   190|    }
   191|
   192|    html << "<hr>\n<p style=\"color:#999;text-align:center;\">Exported with Progressive Chat</p>\n";
   193|    html << "</body>\n</html>";
   194|    return html.str();
   195|}
   196|
   197|std::string buildPlainTextDocument(
   198|    const std::string& roomName,
   199|    const std::string& exportDate,
   200|    const std::vector<ExportEvent>& events
   201|) {
   202|    std::ostringstream text;
   203|
   204|    text << roomName << "\n";
   205|    for (size_t i = 0; i < roomName.size(); ++i) text << "=";
   206|    text << "\n";
   207|    text << "Exported: " << exportDate << "\n";
   208|    text << "Messages: " << events.size() << "\n\n";
   209|
   210|    for (const auto& event : events) {
   211|        text << formatEventPlainText(event);
   212|    }
   213|
   214|    return text.str();
   215|}
   216|
   217|std::string buildJsonDocument(
   218|    const std::string& roomName,
   219|    const std::string& roomTopic,
   220|    const std::string& exportDate,
   221|    const std::string& roomCreator,
   222|    const std::vector<ExportEvent>& events
   223|) {
   224|    std::ostringstream json;
   225|
   226|    json << "{\n";
   227|    json << "  \"roomName\": \"" << escapeJson(roomName) << "\",\n";
   228|    if (!roomTopic.empty())
   229|        json << "  \"roomTopic\": \"" << escapeJson(roomTopic) << "\",\n";
   230|    if (!roomCreator.empty())
   231|        json << "  \"roomCreator\": \"" << escapeJson(roomCreator) << "\",\n";
   232|    json << "  \"exportDate\": \"" << escapeJson(exportDate) << "\",\n";
   233|    json << "  \"messageCount\": " << events.size() << ",\n";
   234|    json << "  \"messages\": [\n";
   235|
   236|    for (size_t i = 0; i < events.size(); ++i) {
   237|        json << formatEventJson(events[i]);
   238|        if (i + 1 < events.size()) json << ",";
   239|        json << "\n";
   240|    }
   241|
   242|    json << "  ]\n";
   243|    json << "}\n";
   244|
   245|    return json.str();
   246|}
   247|
   248|} // namespace progressive
   249|