// Module stubs — satisfy linker for JNI functions until real .cpp files compile
#include "progressive/device_manager_full.hpp"
#include "progressive/room_state_manager.hpp"
#include "progressive/server_notice_manager.hpp"
#include "progressive/composer_manager.hpp"
#include "progressive/content_utils.hpp"
#include "progressive/cross_signing_manager.hpp"
#include "progressive/login_flow.hpp"
#include "progressive/poll_manager.hpp"
#include "progressive/room_directory_manager.hpp"

namespace progressive {

// DeviceManager
DeviceManager::DeviceManager() {}
DeviceManager::~DeviceManager() {}
std::string DeviceManager::devicesToJson() const { return "[]"; }
bool DeviceManager::getDevice(const std::string&, ManagedDeviceInfo&) const { return false; }
std::string DeviceManager::renameDevice(const std::string&, const std::string&, std::string&) { return ""; }
std::string DeviceManager::deleteDevice(const std::string&, std::string&) { return ""; }
std::string DeviceManager::deleteDevices(const std::vector<std::string>&, std::string&) { return ""; }
void DeviceManager::loadDevices(const std::string&) {}

// RoomStateManager
RoomStateManager::RoomStateManager() {}
RoomStateManager::~RoomStateManager() {}
void RoomStateManager::loadState(const std::string&, const std::string&) {}
std::string RoomStateManager::getStateJson(const std::string&) const { return "{}"; }
bool RoomStateManager::isPublicRoom(const std::string&) const { return false; }
bool RoomStateManager::isInviteOnly(const std::string&) const { return false; }
void RoomStateManager::setHistoryVisibility(const std::string&, RSM_RoomHistoryVisibility) {}
void RoomStateManager::setJoinRule(const std::string&, RoomJoinRule) {}

// ServerNoticeManager
ServerNoticeManager::ServerNoticeManager() {}
ServerNoticeManager::~ServerNoticeManager() {}
bool ServerNoticeManager::isResourceLimitError(const std::string&) const { return false; }
bool ServerNoticeManager::isRateLimitError(const std::string&) const { return false; }
bool ServerNoticeManager::isConsentError(const std::string&) const { return false; }
std::string ServerNoticeManager::getBannerColor(int64_t) const { return ""; }
std::string ServerNoticeManager::formatDowntime(int64_t) const { return ""; }

// ComposerManager
ComposerManager::ComposerManager() {}
ComposerManager::~ComposerManager() {}
std::string ComposerManager::applyBold(const std::string&, int, int) { return ""; }
std::string ComposerManager::applyItalic(const std::string&, int, int) { return ""; }
void ComposerManager::setText(const std::string&) {}
void ComposerManager::enterRegularMode(bool) {}
void ComposerManager::enterReplyMode(const std::string&) {}
void ComposerManager::enterEditMode(const std::string&) {}
void ComposerManager::enterQuoteMode(const std::string&) {}
std::string ComposerManager::stateToJson() const { return "{}"; }

// ContentUtils
std::string buildMxcUri(const std::string&, const std::string&) { return ""; }
bool canGenerateThumbnail(const std::string&) { return false; }
std::string autoReplaceEmojis(const std::string& t) { return t; }
std::string buildQuotedBody(const std::string&, const std::string&, const std::string&) { return ""; }

// CrossSigningManager
CrossSigningManager::CrossSigningManager() {}
CrossSigningManager::~CrossSigningManager() {}
bool CrossSigningManager::isInitialized() const { return false; }
bool CrossSigningManager::canCrossSign() const { return false; }
void CrossSigningManager::importPrivateKeys(const std::string&) {}
std::string CrossSigningManager::buildMasterKey(const std::string&) { return ""; }
std::string CrossSigningManager::buildSelfSigningKey(const std::string&) { return ""; }
std::string CrossSigningManager::buildUserSigningKey(const std::string&) { return ""; }
std::string CrossSigningManager::crossSigningInfoToJson() const { return "{}"; }

// LoginFlow
std::string buildPasswordLoginRequest(const LoginCredentials&) { return ""; }

// PollManagerFull
PollManagerFull::PollManagerFull() {}
PollManagerFull::~PollManagerFull() {}
std::string PollManagerFull::createPoll(const std::string&, const std::string&, const std::vector<std::string>&, int, std::string&) { return ""; }
std::string PollManagerFull::castVote(const std::string&, const std::string&, const std::string&, const std::string&, std::string&) { return ""; }
std::string PollManagerFull::endPoll(const std::string&, const std::string&, std::string&) { return ""; }
std::string PollManagerFull::getPollResults(const std::string&) const { return "{}"; }

// RoomDirectoryManager
RoomDirectoryManager::RoomDirectoryManager() {}
RoomDirectoryManager::~RoomDirectoryManager() {}
std::string RoomDirectoryManager::searchPublicRooms(const std::string&, int, const std::string&, bool, const std::string&, std::string&) { return "[]"; }

} // namespace progressive
