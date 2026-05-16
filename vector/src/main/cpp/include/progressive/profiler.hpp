#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <cstdint>

namespace progressive {

// ================================================================
// Native Profiler — measure C++ function execution time
//
// Inspired by Element Android's internal tracing (Timber, Sentry spans)
// and Android Profiler (CPU trace).
//
// Usage:
//   Profiler::start("my_function");
//   // ... code ...
//   Profiler::stop("my_function");
//   // Get report: Profiler::instance().report();
//
// Slash commands:
//   /profile start — begin profiling
//   /profile stop  — stop profiling and print report
//   /profile reset — clear all data
//   /profile status — show current stats
// ================================================================

// ---- Profile Entry (single measurement) ----

struct ProfileEntry {
    std::string name;                // Function/module name
    int64_t startTimeNs = 0;         // Start timestamp (nanoseconds)
    int64_t endTimeNs = 0;           // End timestamp
    int64_t durationNs = 0;          // Duration
    int callCount = 0;               // How many times called
    bool active = false;             // Currently measuring
};

// ---- Profile Summary (aggregated) ----

struct ProfileSummary {
    std::string name;
    int callCount = 0;
    int64_t totalTimeNs = 0;         // Total accumulated time
    int64_t minTimeNs = INT64_MAX;
    int64_t maxTimeNs = 0;
    int64_t avgTimeNs = 0;
    double percentTotal = 0.0;       // % of total profiling time
};

// ---- Profile Report ----

struct ProfileReport {
    std::vector<ProfileSummary> entries;
    int64_t totalTimeNs = 0;         // Total time of all entries
    int64_t reportTimeNs = 0;        // When report was generated
    bool isProfiling = false;        // Currently active
    int activeEntries = 0;           // Currently open entries
    std::string format;              // "text" or "json"
    int64_t overheadNs = 0;          // Profiler's own overhead
};

// ---- Memory Snapshot ----

struct MemorySnapshot {
    int64_t timestampNs = 0;
    int64_t allocatedBytes = 0;      // Approximate allocated memory
    int allocateCount = 0;
    int deallocateCount = 0;
    std::string label;
};

// ---- Profiler ----

class Profiler {
public:
    // Singleton access.
    static Profiler& instance();

    // ====== Lifecycle ======

    // Start profiling.
    void startProfiling();

    // Stop profiling.
    void stopProfiling();

    // Reset all data.
    void reset();

    // Check if profiling is active.
    bool isProfiling() const { return profiling_; }

    // ====== Measurement ======

    // Start timing a named entry.
    // Returns the entry index (for stop-by-index).
    int start(const std::string& name);

    // Stop timing by name. Returns duration in nanoseconds.
    int64_t stop(const std::string& name);

    // Stop timing by entry index. Returns duration in nanoseconds.
    int64_t stop(int entryIndex);

    // Time a scope (RAII style). Creates a ScopeTimer that auto-stops on destruction.
    // Usage: auto timer = Profiler::scope("my_func");
    // The timer stops when it goes out of scope.

    // ====== Scoped Timer (RAII) ======
    class ScopeTimer {
    public:
        ScopeTimer(const std::string& name);
        ~ScopeTimer();
        int64_t elapsedNs() const;
    private:
        std::string name_;
        int64_t startNs_;
        bool stopped_ = false;
    };

    // Convenience: creates a ScopeTimer.
    static ScopeTimer scope(const std::string& name) { return ScopeTimer(name); }

    // ====== Memory Tracking ======

    // Record a memory allocation.
    void recordAlloc(int64_t bytes);

    // Record a memory deallocation.
    void recordDealloc(int64_t bytes);

    // Take a memory snapshot.
    MemorySnapshot takeMemorySnapshot(const std::string& label = "");

    // Get current tracked memory.
    int64_t currentTrackedMemory() const { return trackedMemory_; }

    // ====== Reporting ======

    // Generate a full profile report.
    ProfileReport generateReport() const;

    // Format report as JSON string.
    std::string reportToJson() const;

    // Format report as human-readable text.
    std::string reportToText() const;

    // Format a single summary entry.
    std::string summaryToJson(const ProfileSummary& s) const;

    // Get summary for a specific entry name.
    ProfileSummary getSummary(const std::string& name) const;

    // ====== Stats ======

    int totalEntries() const { return static_cast<int>(entries_.size()); }
    int totalSummaries() const { return static_cast<int>(summaries_.size()); }
    int64_t totalProfiledTime() const;
    int getActiveCount() const;

private:
    Profiler();
    ~Profiler() = default;
    Profiler(const Profiler&) = delete;
    Profiler& operator=(const Profiler&) = delete;

    bool profiling_ = false;
    std::vector<ProfileEntry> entries_;                          // Raw entries
    std::unordered_map<std::string, ProfileSummary> summaries_;  // Aggregated
    std::vector<MemorySnapshot> memorySnapshots_;
    int64_t trackedMemory_ = 0;                                  // Current tracked memory
    int allocCount_ = 0;
    int deallocCount_ = 0;

    // High-resolution timer.
    static int64_t nowNs();

    // Overhead calibration.
    int64_t measureOverhead();
    int64_t overheadNs_ = 0;

    // Format nanoseconds as human-readable string.
    static std::string formatNs(int64_t ns);
    static std::string formatBytes(int64_t bytes);
};

} // namespace progressive
