#pragma once
#include <string>
#include <vector>
#include <deque>
#include <cstdint>

namespace progressive {

struct QueuedMessage {
    std::string id;
    std::string roomId;
    std::string body;
    std::string formattedBody;
    std::string msgType;        // "m.text", "m.image", etc.
    int retryCount = 0;
    int maxRetries = 5;
    int64_t queuedAtMs = 0;
    int64_t nextRetryMs = 0;
    bool failed = false;
    std::string errorMessage;
};

class MessageQueue {
public:
    MessageQueue();

    void enqueue(const QueuedMessage& msg);
    void enqueue(const std::string& roomId, const std::string& body,
                 const std::string& msgType = "m.text");

    QueuedMessage dequeue();
    bool isEmpty() const;
    size_t size() const;
    int pendingCount() const;

    // Get due messages (nextRetryMs <= now)
    std::vector<QueuedMessage> getDue(int64_t nowMs = 0);

    void markFailed(const std::string& id, const std::string& error);
    void markSent(const std::string& id);
    void retry(const std::string& id);

    // Serialization
    std::string toJson() const;
    void fromJson(const std::string& json);

private:
    std::deque<QueuedMessage> queue_;
    int nextId_ = 1;
};

} // namespace progressive
