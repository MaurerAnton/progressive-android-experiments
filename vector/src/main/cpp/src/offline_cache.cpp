#include "progressive/offline_cache.hpp"
#include <sstream>
#include <algorithm>
#include <cmath>
#include <algorithm>
#include <sstream>

namespace progressive {

// ==== Priority Computation ====

int computeRoomCacheScore(const RoomPriority& room) {
    int score = room.priority;

    // Original logic: priority base (0-100) + bonuses
    if (room.isFavourite) score += 30;
    if (room.isDirect) score += 20;
    if (room.isPinned) score += 40;

    // Recency bonus: rooms active in last 24h get up to +20
    auto now = static_cast<int64_t>(std::time(nullptr)) * 1000;
    auto ageHours = (now - room.lastActivityMs) / (1000 * 60 * 60);
    if (ageHours < 1) score += 20;
    else if (ageHours < 6) score += 15;
    else if (ageHours < 24) score += 10;
    else if (ageHours < 72) score += 5;
    else if (ageHours > 168) score -= 10; // > 1 week — penalty

    // Message/media bonus: rooms with more content get slight priority
    if (room.messageCount > 1000) score += 5;
    if (room.mediaFileCount > 100) score += 3;

    return std::max(0, std::min(score, 200)); // clamp 0-200
}

std::vector<RoomPriority> prioritizeRooms(
    std::vector<RoomPriority> rooms, int topN)
{
    // Sort by score descending, filter out priority=0
    std::sort(rooms.begin(), rooms.end(),
        [](const RoomPriority& a, const RoomPriority& b) {
            return computeRoomCacheScore(a) > computeRoomCacheScore(b);
        });

    // Remove rooms with score=0
    rooms.erase(
        std::remove_if(rooms.begin(), rooms.end(),
            [](const RoomPriority& r) { return computeRoomCacheScore(r) <= 0; }),
        rooms.end());

    if (static_cast<int>(rooms.size()) > topN) {
        rooms.resize(topN);
    }

    return rooms;
}

// ==== Budget Allocation ====

OfflineCachePlan allocateCacheBudget(
    const std::vector<RoomPriority>& rooms,
    const OfflineCacheConfig& config)
{
    OfflineCachePlan plan;

    // Available budget
    int64_t usable = config.availableStorage - config.reservedStorage;
    if (usable <= 0) return plan;
    plan.totalBudget = usable;

    auto prioritized = prioritizeRooms(
        const_cast<std::vector<RoomPriority>&>(rooms), config.roomLimit);

    // Calculate total priority weight
    int totalWeight = 0;
    for (const auto& r : prioritized) {
        totalWeight += computeRoomCacheScore(r);
    }
    if (totalWeight == 0) return plan;

    // Allocate proportional to priority score
    for (const auto& room : prioritized) {
        RoomCachePlan rp;
        rp.roomId = room.roomId;
        rp.roomName = room.roomName;

        // Proportional budget
        int weight = computeRoomCacheScore(room);
        rp.allocatedBytes = usable * weight / totalWeight;

        // How many messages can we cache with this budget?
        int64_t msgMem = estimateMessageCacheSize(std::min(
            room.messageCount, config.maxMessagesPerRoom));
        if (msgMem <= rp.allocatedBytes) {
            rp.messagesToDownload = std::min(room.messageCount, config.maxMessagesPerRoom);
            rp.canFitAllMessages = true;
        } else {
            rp.messagesToDownload = static_cast<int>(
                rp.allocatedBytes / estimateMessageCacheSize(1));
            rp.canFitAllMessages = false;
        }

        // How many media files can we cache?
        int64_t medMem = estimateMediaCacheSize(room.mediaFileCount);
        if (medMem > 0 && rp.allocatedBytes > msgMem) {
            int64_t mediaBudget = rp.allocatedBytes - msgMem;
            if (mediaBudget >= medMem) {
                rp.mediaFilesToDownload = room.mediaFileCount;
                rp.canFitAllMedia = true;
            } else {
                rp.mediaFilesToDownload = static_cast<int>(
                    mediaBudget / estimateMediaCacheSize(1));
                rp.canFitAllMedia = false;
            }
        }

        plan.totalAllocated += rp.allocatedBytes;
        plan.roomPlans.push_back(rp);
        plan.roomsCached++;
    }

    // Estimate download time (rough: 1MB/s on average mobile)
    plan.estimatedTimeMs = plan.totalAllocated / 1024; // ms per KB

    return plan;
}

// ==== Media Filtering ====

bool shouldCacheMedia(const OfflineCacheConfig& config,
    const std::string& mimeType, int64_t fileSize)
{
    // Size limit
    if (fileSize > config.maxFileSize) return false;

    // Type filter
    if (mimeType.find("image/") == 0) return config.downloadImages;
    if (mimeType.find("video/") == 0) return config.downloadVideos;
    if (mimeType.find("audio/") == 0) return config.downloadAudio;
    if (mimeType.find("application/") == 0 || mimeType.find("text/") == 0)
        return config.downloadFiles;

    return false; // Unknown type — skip
}

// ==== Size Estimation ====

int64_t estimateMessageCacheSize(int messageCount, int avgBodySize) {
    // Rough: message body + metadata ~ avgBodySize + 200 bytes overhead
    return static_cast<int64_t>(messageCount) * (avgBodySize + 200);
}

int64_t estimateMediaCacheSize(int mediaCount, int64_t avgMediaSize) {
    return static_cast<int64_t>(mediaCount) * avgMediaSize;
}

bool canFitInStorage(int64_t required, int64_t available, int64_t reserved) {
    return (available - reserved) >= required;
}

// ==== JSON ====

std::string roomPriorityToJson(const RoomPriority& room) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"roomId": ")" << esc(room.roomId) << R"(",)";
    json << R"("priority": )" << room.priority << ",";
    json << R"("score": )" << computeRoomCacheScore(room) << ",";
    json << R"("messageCount": )" << room.messageCount << ",";
    json << R"("mediaFileCount": )" << room.mediaFileCount << "}";
    return json.str();
}

std::string offlineCachePlanToJson(const OfflineCachePlan& plan) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"totalBudget": )" << plan.totalBudget << ",";
    json << R"("totalAllocated": )" << plan.totalAllocated << ",";
    json << R"("roomsCached": )" << plan.roomsCached << ",";
    json << R"("roomsSkipped": )" << plan.roomsSkipped << ",";
    json << R"("estimatedTimeMs": )" << plan.estimatedTimeMs << ",";
    json << R"("roomPlans": [)";
    for (size_t i = 0; i < plan.roomPlans.size(); ++i) {
        if (i > 0) json << ",";
        const auto& rp = plan.roomPlans[i];
        json << R"({"roomId": ")" << esc(rp.roomId) << R"(",)";
        json << R"("allocatedBytes": )" << rp.allocatedBytes << ",";
        json << R"("messagesToDownload": )" << rp.messagesToDownload << ",";
        json << R"("mediaFilesToDownload": )" << rp.mediaFilesToDownload << ",";
        json << R"("canFitAllMessages": )" << (rp.canFitAllMessages ? "true" : "false");
        json << "}";
    }
    json << "]}";
    return json.str();
}

} // namespace progressive
