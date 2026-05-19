#include "progressive/eventdb.hpp"

// Note: This implementation uses sqlite3 C API.
// SQLite amalgamation is available at build time under vector/.cxx/
// but the header path resolution is not yet configured in this module.
// For now, this is a stub that compiles without sqlite3.
// Uncomment when sqlite3 include path is resolved.
// #include <sqlite3.h>

namespace progressive {

EventDatabase::EventDatabase() = default;

EventDatabase::~EventDatabase() {
    close();
}

bool EventDatabase::open(const std::string& /*dbPath*/) {
    // SQLite stub — not yet linked
    return false;
}

void EventDatabase::close() {
    // SQLite stub
}

bool EventDatabase::initSchema() {
    return false;
}

void EventDatabase::insertEvent(const DbEvent& /*event*/) {}

void EventDatabase::insertAnnotations(const DbAnnotations& /*annotations*/) {}

std::string EventDatabase::getContextJson(const std::string& /*eventId*/) const {
    return R"({"cached": false})";
}

std::vector<DbEvent> EventDatabase::getEvents(const std::string& /*roomId*/, int /*limit*/, int /*offset*/) const {
    return {};
}

void EventDatabase::clearRoom(const std::string& /*roomId*/) {}

void EventDatabase::clearAll() {}

int EventDatabase::count() const {
    return 0;
}

void EventDatabase::exec(const char* /*sql*/) const {}

} // namespace progressive
