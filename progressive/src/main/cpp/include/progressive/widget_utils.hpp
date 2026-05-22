#pragma once
#include <string>
#include <vector>

namespace progressive {

struct WidgetInfo {
    std::string widgetId;
    std::string type;           // e.g. "m.custom", "jitsi"
    std::string name;
    std::string url;
    std::string creatorUserId;
    std::string avatarUrl;
    bool waitForIframeLoad = true;
};

struct WidgetAction {
    std::string action;         // e.g. "send_event", "open_id"
    std::string widgetId;
    std::string data;
    std::string requestId;
};

// Parse widget from m.widget state event
WidgetInfo parseWidget(const std::string& stateKey, const std::string& contentJson);

// Build widget state event content
std::string buildWidgetContent(const WidgetInfo& widget);

// Parse widget action from widget API request
WidgetAction parseWidgetAction(const std::string& apiJson);

// Build widget action response
std::string buildWidgetActionResponse(const WidgetAction& action, const std::string& result);

// Format widget for display
std::string formatWidgetInfo(const WidgetInfo& widget);

// Validate widget URL (must be HTTPS)
bool isValidWidgetUrl(const std::string& url);

} // namespace progressive
