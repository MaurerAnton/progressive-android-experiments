#include "progressive/eventcache.hpp"
#include <sstream>
#include <android/log.h>

#define LOG_TAG "EventCache"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

namespace progressive {

void EventCache::put(const CachedEvent& event) {
    // Update reverse index: remove old relation if any
    auto it = events_.find(event.eventId);
    if (it != events_.end() && !it->second.sourceEventId.empty()) {
        auto& vec = relationIndex_[it->second.sourceEventId];
        vec.erase(std::remove(vec.begin(), vec.end(), event.eventId), vec.end());
    }

    // Store the event
    events_[event.eventId] = event;

    // Update reverse index
    if (!event.sourceEventId.empty()) {
        relationIndex_[event.sourceEventId].push_back(event.eventId);
    }
}

const CachedEvent* EventCache::get(const std::string& eventId) const {
    auto it = events_.find(eventId);
    if (it != events_.end()) return &it->second;
    return nullptr;
}

std::vector<const CachedEvent*> EventCache::getRelations(const std::string& sourceEventId) const {
    std::vector<const CachedEvent*> results;
    auto idxIt = relationIndex_.find(sourceEventId);
    if (idxIt != relationIndex_.end()) {
        for (const auto& relatedId : idxIt->second) {
            auto eventIt = events_.find(relatedId);
            if (eventIt != events_.end()) {
                results.push_back(&eventIt->second);
            }
        }
    }
    return results;
}

void EventCache::clear() {
    events_.clear();
    relationIndex_.clear();
}

std::string EventCache::getContextData(const std::string& eventId) const {
    auto it = events_.find(eventId);
    if (it == events_.end()) {
        return R"({"cached": false})";
    }

    const auto& e = it->second;
    std::ostringstream json;

    json << "{";
    json << R"("cached": true,)";
    json << R"("eventId": ")" << e.eventId << R"(",)";
    json << R"("senderId": ")" << e.senderId << R"(",)";
    json << R"("senderName": ")" << e.senderName << R"(",)";
    json << R"("timestamp": ")" << e.timestamp << R"(",)";
    json << R"("body": ")" << e.body << R"(",)";
    json << R"("msgType": ")" << e.msgType << R"(",)";
    json << R"("eventType": ")" << e.eventType << R"(",)";
    json << R"("sentByMe": )" << (e.sentByMe ? "true" : "false") << ",";
    json << R"("hasFailed": )" << (e.hasFailed ? "true" : "false") << ",";
    json << R"("reactionCount": )" << e.reactionCount << ",";
    json << R"("hasBeenEdited": )" << (e.hasBeenEdited ? "true" : "false");

    // Add relation info if present
    if (!e.relationType.empty()) {
        json << R"(,"relationType": ")" << e.relationType << R"(")";
        json << R"(,"sourceEventId": ")" << e.sourceEventId << R"(")";
    }

    // Add reactions summary
    auto relations = getRelations(eventId);
    if (!relations.empty()) {
        json << R"(,"reactions": [)";
        for (size_t i = 0; i < relations.size(); ++i) {
            if (i > 0) json << ",";
            json << R"({"key": ")" << relations[i]->body
                 << R"(", "count": )" << relations[i]->reactionCount
                 << R"(", "addedByMe": )" << (relations[i]->sentByMe ? "true" : "false")
                 << "}";
        }
        json << "]";
    }

    json << "}";
    return json.str();
}



// ---- Extended Cache Management ----

void EventCache::configure(const EventCacheConfig& config) {
    config_ = config;
}

const EventCacheConfig& EventCache::getConfig() const {
    return config_;
}

EventCacheStats EventCache::getCacheStats() const {
    EventCacheStats stats;
    stats.hits = hits_;
    stats.misses = misses_;
    stats.evictions = evictions_;
    stats.size = entries_.size();
    int64_t total = stats.hits + stats.misses;
    stats.hitRate = total > 0 ? static_cast<double>(stats.hits) / static_cast<double>(total) : 0.0;
    return stats;
}

void EventCache::evictByPolicy() {
    if (entries_.empty()) return;
    evictions_++;
    if (config_.policy == EventCachePolicy::FIFO) {
        entries_.erase(entries_.begin());
    } else {
        // LRU: remove the one with oldest lastAccessedAt
        auto oldest = entries_.begin();
        int64_t oldestTime = INT64_MAX;
        for (auto it = entries_.begin(); it != entries_.end(); ++it) {
            if (it->second.meta.lastAccessedAt < oldestTime) {
                oldestTime = it->second.meta.lastAccessedAt;
                oldest = it;
            }
        }
        entries_.erase(oldest);
    }
}

bool EventCache::isEvictionNeeded() const {
    return entries_.size() >= config_.maxSize;
}

void EventCache::warmCache(const std::vector<CachedEvent>& events) {
    for (const auto& ev : events) {
        entries_[ev.eventId] = ev;
    }
}

void EventCache::invalidateRoom(const std::string& roomId) {
    for (auto it = entries_.begin(); it != entries_.end(); ) {
        if (it->second.roomId == roomId) {
            it = entries_.erase(it);
        } else {
            ++it;
        }
    }
}

void EventCache::invalidateSender(const std::string& senderId) {
    for (auto it = entries_.begin(); it != entries_.end(); ) {
        if (it->second.senderId == senderId) {
            it = entries_.erase(it);
        } else {
            ++it;
        }
    }
}

double EventCache::getCacheHitRate() const {
    auto stats = getCacheStats();
    return stats.hitRate;
}

} // namespace progressive
