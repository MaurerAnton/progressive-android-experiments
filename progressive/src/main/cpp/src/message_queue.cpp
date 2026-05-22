#include "progressive/message_queue.hpp"
#include <sstream>
#include <chrono>
#include <algorithm>

namespace progressive {

MessageQueue::MessageQueue() {}

static int64_t now() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

void MessageQueue::enqueue(const QueuedMessage& msg) {
    QueuedMessage qm = msg;
    if (qm.id.empty()) qm.id = "msg_" + std::to_string(nextId_++);
    if (qm.queuedAtMs == 0) qm.queuedAtMs = now();
    if (qm.nextRetryMs == 0) qm.nextRetryMs = qm.queuedAtMs;
    queue_.push_back(qm);
}

void MessageQueue::enqueue(const std::string& roomId, const std::string& body, const std::string& msgType) {
    QueuedMessage qm;
    qm.id = "msg_" + std::to_string(nextId_++);
    qm.roomId = roomId;
    qm.body = body;
    qm.msgType = msgType;
    qm.queuedAtMs = now();
    qm.nextRetryMs = qm.queuedAtMs;
    queue_.push_back(qm);
}

QueuedMessage MessageQueue::dequeue() {
    if (queue_.empty()) return {};
    auto msg = queue_.front();
    queue_.pop_front();
    return msg;
}

bool MessageQueue::isEmpty() const { return queue_.empty(); }
size_t MessageQueue::size() const { return queue_.size(); }
int MessageQueue::pendingCount() const {
    return (int)std::count_if(queue_.begin(), queue_.end(),
        [](const QueuedMessage& m) { return !m.failed; });
}

std::vector<QueuedMessage> MessageQueue::getDue(int64_t nowMs) {
    if (nowMs <= 0) nowMs = now();
    std::vector<QueuedMessage> due;
    for (auto& m : queue_) {
        if (!m.failed && m.nextRetryMs <= nowMs && m.retryCount < m.maxRetries) {
            due.push_back(m);
        }
    }
    return due;
}

void MessageQueue::markFailed(const std::string& id, const std::string& error) {
    for (auto& m : queue_) {
        if (m.id == id) {
            m.retryCount++;
            m.errorMessage = error;
            if (m.retryCount >= m.maxRetries) m.failed = true;
            else m.nextRetryMs = now() + (1LL << m.retryCount) * 1000; // Exponential backoff
            break;
        }
    }
}

void MessageQueue::markSent(const std::string& id) {
    queue_.erase(std::remove_if(queue_.begin(), queue_.end(),
        [&](const QueuedMessage& m) { return m.id == id; }), queue_.end());
}

void MessageQueue::retry(const std::string& id) {
    for (auto& m : queue_) {
        if (m.id == id) {
            m.failed = false;
            m.retryCount = 0;
            m.nextRetryMs = now();
            break;
        }
    }
}

std::string MessageQueue::toJson() const {
    std::ostringstream os;
    os << "[";
    for (size_t i = 0; i < queue_.size(); i++) {
        if (i > 0) os << ",";
        const auto& m = queue_[i];
        os << R"({)"";
        os << R"("id":")" << m.id << R"(")";
        os << R"(,"roomId":")" << m.roomId << R"(")";
        os << R"(,"body":")" << m.body << R"(")";
        os << R"(,"retryCount":)" << m.retryCount;
        os << R"(,"failed":)" << (m.failed ? "true" : "false");
        os << "}";
    }
    os << "]";
    return os.str();
}

void MessageQueue::fromJson(const std::string& json) {
    queue_.clear();
    if (json.empty() || json == "[]") return;
    // Simple parse — just clear and let new messages be queued
}

} // namespace progressive
