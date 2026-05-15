#pragma once

#include <string>
#include "progressive/http_client.hpp"
#include "progressive/sync_models.hpp"
#include "progressive/auth_models.hpp"

namespace progressive {

// ==== Native Matrix API Client ====
//
// C++ replacements for Retrofit API calls. Each function maps to a
// Matrix Client-Server API endpoint. Used when SETTINGS_LABS_NATIVE_HTTP is ON.
// Original Kotlin Retrofit code remains as fallback.
//
// All functions are synchronous (blocking). The caller is responsible for
// running them on a background thread.

// Base URL for the homeserver (set once during init).
void setHomeserverBaseUrl(const std::string& url);
const std::string& getHomeserverBaseUrl();
void setAccessToken(const std::string& token);

// ==== Auth API ====

// GET /_matrix/client/versions — server version support
// Returns raw JSON response body
std::string apiGetVersions();

// GET /_matrix/client/r0/login — get available login flows
std::string apiGetLoginFlows();

// POST /_matrix/client/r0/login — authenticate with password
// Body: {"type":"m.login.password","identifier":{"type":"m.id.user","user":"..."},"password":"..."}
Credentials apiLogin(const std::string& userId, const std::string& password,
                     const std::string& deviceId = "");

// POST /_matrix/client/r0/register — register a new account
std::string apiRegister(const std::string& username, const std::string& password,
                        const std::string& deviceId = "");

// GET /_matrix/client/r0/register/available — check if username is available
bool apiUsernameAvailable(const std::string& username);

// ==== Sync API ====

// GET /_matrix/client/r0/sync — full sync request
// filter: JSON filter string or empty
// since: pagination token or empty for initial sync
// timeout: long-poll timeout in ms (0 = no polling)
SyncResponse apiSync(const std::string& filter = "",
                     const std::string& since = "",
                     int timeout = 30000);

// ==== Room API ====

// POST /_matrix/client/r0/createRoom — create a new room
std::string apiCreateRoom(const std::string& name = "",
                          const std::string& topic = "",
                          bool isDirect = false,
                          const std::vector<std::string>& inviteUsers = {});

// GET /_matrix/client/r0/rooms/{roomId}/messages — get room messages
// from: pagination token (empty = latest)
// dir: "b" = backwards, "f" = forwards
// limit: max events to return
std::string apiGetRoomMessages(const std::string& roomId,
                               const std::string& from = "",
                               const std::string& dir = "b",
                               int limit = 20);

// PUT /_matrix/client/r0/rooms/{roomId}/send/{eventType}/{txnId} — send event
std::string apiSendEvent(const std::string& roomId,
                         const std::string& eventType,
                         const std::string& txnId,
                         const std::string& contentJson);

// POST /_matrix/client/r0/rooms/{roomId}/join — join room
std::string apiJoinRoom(const std::string& roomId, const std::string& reason = "");

// POST /_matrix/client/r0/rooms/{roomId}/leave — leave room
std::string apiLeaveRoom(const std::string& roomId);

// ==== Profile API ====

// GET /_matrix/client/r0/profile/{userId} — get user profile
std::string apiGetProfile(const std::string& userId);

// GET /_matrix/client/r0/profile/{userId}/displayname
std::string apiGetDisplayName(const std::string& userId);

// PUT /_matrix/client/r0/profile/{userId}/displayname
std::string apiSetDisplayName(const std::string& userId, const std::string& displayName);

// ==== Media API ====

// POST /_matrix/media/r0/upload — upload file
std::string apiUploadMedia(const std::string& fileName,
                           const std::string& contentType,
                           const std::vector<uint8_t>& data);

// ==== Utility ====

// Get the status of the native HTTP client.
// Returns true if the TLS bridge and homeserver URL are configured.
bool nativeApiAvailable();

} // namespace progressive
