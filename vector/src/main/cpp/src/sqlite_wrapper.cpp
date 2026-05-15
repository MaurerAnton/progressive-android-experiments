#include "progressive/sqlite_wrapper.hpp"
#include <cstring>

// Note: This implementation uses the sqlite3 C API.
// On Android NDK, sqlite3 is available via libsqlite or
// linked from the system. Include the NDK sqlite header.
// For now, we provide a stub that compiles without sqlite3.

// Uncomment when sqlite3 is linked:
// #include <sqlite3.h>

namespace progressive {

// ==== SQLite Stub — requires linking libsqlite ====
// The full implementation uses sqlite3_open, sqlite3_exec, etc.
// This compiles as a placeholder until sqlite3 is linked.

SqliteDB SqliteDB::open(const std::string& /*path*/) {
    SqliteDB db;
    // db.db_ = nullptr; // sqlite3_open(path, &db) would go here
    return db;
}

SqliteDB::~SqliteDB() {
    // sqlite3_close(db_);
}

bool SqliteDB::execute(const std::string& /*sql*/) {
    return false; // Stub
}

bool SqliteDB::createTimelineSchema() {
    // CREATE TABLE IF NOT EXISTS events (
    //   event_id TEXT PRIMARY KEY,
    //   room_id TEXT NOT NULL,
    //   type TEXT,
    //   sender_id TEXT,
    //   content_json TEXT,
    //   origin_server_ts INTEGER,
    //   age_local_ts INTEGER,
    //   display_index INTEGER,
    //   state_key TEXT DEFAULT '',
    //   redacts TEXT DEFAULT '',
    //   relation_type TEXT DEFAULT '',
    //   relates_to_id TEXT DEFAULT ''
    // );
    // CREATE INDEX idx_events_room ON events(room_id, display_index);
    return false;
}

int SqliteDB::schemaVersion() {
    // PRAGMA user_version;
    return 0;
}

void SqliteDB::setSchemaVersion(int /*version*/) {
    // PRAGMA user_version = version;
}

bool SqliteDB::insertEvent(
    const std::string& /*eventId*/, const std::string& /*roomId*/,
    const std::string& /*type*/, const std::string& /*senderId*/,
    const std::string& /*contentJson*/, int64_t /*originServerTs*/,
    int64_t /*ageLocalTs*/, int /*displayIndex*/,
    const std::string& /*stateKey*/, const std::string& /*redacts*/,
    const std::string& /*relationType*/, const std::string& /*relatesToId*/)
{
    return false;
}

bool SqliteDB::deleteEvent(const std::string& /*eventId*/) {
    return false;
}

std::vector<SqliteDB::EventRow> SqliteDB::queryEvents(
    const std::string& /*roomId*/, int /*limit*/, int /*offset*/, bool /*ascending*/)
{
    return {};
}

SqliteDB::EventRow SqliteDB::queryEvent(const std::string& /*eventId*/) {
    return {};
}

int SqliteDB::countEvents(const std::string& /*roomId*/) { return 0; }
int SqliteDB::maxDisplayIndex(const std::string& /*roomId*/) { return 0; }

bool SqliteDB::upsertRoom(
    const std::string& /*roomId*/, const std::string& /*displayName*/,
    const std::string& /*avatarUrl*/, const std::string& /*topic*/,
    const std::string& /*membership*/, int /*notificationCount*/,
    int /*highlightCount*/, int64_t /*lastActivityMs*/,
    bool /*isDirect*/, bool /*isSpace*/, bool /*isFavourite*/,
    bool /*isEncrypted*/)
{
    return false;
}

std::vector<SqliteDB::RoomRow> SqliteDB::queryRooms() { return {}; }

void SqliteDB::beginTransaction() {}
void SqliteDB::commitTransaction() {}
void SqliteDB::rollbackTransaction() {}

// ==== SQL Helpers ====

std::string sqlEscape(const std::string& s) {
    std::string result;
    for (char c : s) {
        if (c == '\'') result += "''";
        else result += c;
    }
    return result;
}

std::string buildInsertSql(
    const std::string& table,
    const std::vector<std::string>& columns,
    const std::vector<std::string>& values)
{
    std::string sql = "INSERT OR REPLACE INTO " + table + " (";
    for (size_t i = 0; i < columns.size(); i++) {
        if (i > 0) sql += ", ";
        sql += columns[i];
    }
    sql += ") VALUES (";
    for (size_t i = 0; i < values.size(); i++) {
        if (i > 0) sql += ", ";
        sql += values[i];
    }
    sql += ")";
    return sql;
}

} // namespace progressive
