#include "progressive/composer_manager.hpp"
#include "progressive/content_utils.hpp"
#include "progressive/cross_signing_manager.hpp"
#include "progressive/device_manager_full.hpp"
#include "progressive/poll_manager.hpp"
#include "progressive/room_directory_manager.hpp"
#include "progressive/room_state_manager.hpp"
#include "progressive/server_notice_manager.hpp"
#include "progressive/space_graph.hpp"

namespace progressive {

MentionMatch::MentionMatch() {}
MentionMatch::~MentionMatch() {}
EmojiMatch::EmojiMatch() {}
EmojiMatch::~EmojiMatch() {}
MessageValidation::MessageValidation() {}
MessageValidation::~MessageValidation() {}
void ComposerManager::enterRegularMode(bool fromSharing = false) {}
void ComposerManager::enterEditMode(const std::string& eventId) {}
void ComposerManager::enterQuoteMode(const std::string& eventId) {}
void ComposerManager::enterReplyMode(const std::string& eventId) {}
void ComposerManager::enterVoiceMode() {}
SendMode ComposerManager::getSendMode() { return {}; }
SendModeType ComposerManager::getSendModeType() { return {}; }
void ComposerManager::setText(const std::string& text) {}
string ComposerManager::getText() { return {}; }
string ComposerManager::insertMention(int cursorPos, const MentionMatch& match) { return {}; }
void ComposerManager::setFullScreen(bool fullScreen) {}
string ComposerManager::applyBold(const std::string& text, int selStart, int selEnd) { return {}; }
string ComposerManager::applyItalic(const std::string& text, int selStart, int selEnd) { return {}; }
string ComposerManager::applyCode(const std::string& text, int selStart, int selEnd) { return {}; }
string ComposerManager::applyQuote(const std::string& text, int selStart, int selEnd) { return {}; }
bool ComposerManager::canSendMessage() { return {}; }
MessageValidation ComposerManager::validateCurrentMessage() { return {}; }
string ComposerManager::stateToJson() { return {}; }
string ComposerManager::sendModeToJson(const SendMode& mode) { return {}; }
ComposerManager::ComposerManager() {}
ComposerManager::~ComposerManager() {}
TextFormat detectTextFormat(const std::string& text) { return {}; }
bool isAtMentionTrigger(const std::string& text, int cursorPos, bool& isRoom) { return {}; }
MessageValidation validateMessage(const std::string& text, int maxLength = 65535) { return {}; }
CSM_KeyUsage::CSM_KeyUsage() {}
CSM_KeyUsage::~CSM_KeyUsage() {}
Builder::Builder() {}
Builder::~Builder() {}
bool CSM_CrossSigningInfo::isTrusted() { return {}; }
CSM_CrossSigningKey* CSM_CrossSigningInfo::masterKey() { return {}; }
CSM_CrossSigningKey* CSM_CrossSigningInfo::userKey() { return {}; }
CSM_CrossSigningKey* CSM_CrossSigningInfo::selfSigningKey() { return {}; }
CSM_CrossSigningInfo::CSM_CrossSigningInfo() {}
CSM_CrossSigningInfo::~CSM_CrossSigningInfo() {}
UserTrustResult::UserTrustResult() {}
UserTrustResult::~UserTrustResult() {}
DeviceTrustResult::DeviceTrustResult() {}
DeviceTrustResult::~DeviceTrustResult() {}
CS_RoomEncryptionTrustLevel::CS_RoomEncryptionTrustLevel() {}
CS_RoomEncryptionTrustLevel::~CS_RoomEncryptionTrustLevel() {}
bool CrossSigningManager::isInitialized() { return {}; }
bool CrossSigningManager::isVerified() { return {}; }
bool CrossSigningManager::canCrossSign() { return {}; }
bool CrossSigningManager::allPrivateKeysKnown() { return {}; }
void CrossSigningManager::setMyKeys(const CSM_CrossSigningInfo& info) {}
void CrossSigningManager::setUserKeys(const std::string& userId, const CSM_CrossSigningInfo& info) {}
CSM_CrossSigningInfo CrossSigningManager::getMyKeys() { return {}; }
CSM_CrossSigningInfo CrossSigningManager::getUserKeys(const std::string& userId) { return {}; }
UserTrustResult CrossSigningManager::importPrivateKeys(const std::string& masterKeyPrivate, const std::string& userSigningKeyPrivate, const std::string& selfSigningKeyPrivate) { return {}; }
bool CrossSigningManager::importPrivateKey(CSM_KeyUsage usage, const std::string& privateKey) { return {}; }
PrivateKeysInfo CrossSigningManager::getPrivateKeys() { return {}; }
UserTrustResult CrossSigningManager::checkSelfTrust() { return {}; }
UserTrustResult CrossSigningManager::checkUserTrust(const std::string& otherUserId) { return {}; }
DeviceTrustResult CrossSigningManager::checkDeviceTrust(const std::string& userId, const std::string& deviceId, bool locallyTrusted) { return {}; }
void CrossSigningManager::markMyMasterKeyAsTrusted() {}
void CrossSigningManager::trustUser(const std::string& otherUserId) {}
void CrossSigningManager::trustDevice(const std::string& deviceId) {}
CSM_CrossSigningKey CrossSigningManager::buildMasterKey(const std::string& userId, const std::string& publicKey) { return {}; }
CSM_CrossSigningKey CrossSigningManager::buildSelfSigningKey(const std::string& userId, const std::string& publicKey) { return {}; }
CSM_CrossSigningKey CrossSigningManager::buildUserSigningKey(const std::string& userId, const std::string& publicKey) { return {}; }
CSM_CrossSigningInfo CrossSigningManager::buildCrossSigningInfo(const std::string& userId, const CSM_CrossSigningKey& msk, const CSM_CrossSigningKey& usk, const CSM_CrossSigningKey& ssk) { return {}; }
string CrossSigningManager::crossSigningInfoToJson(const CSM_CrossSigningInfo& info) { return {}; }
string CrossSigningManager::keyToJson(const CSM_CrossSigningKey& key) { return {}; }
string CrossSigningManager::trustResultToJson(const UserTrustResult& result) { return {}; }
string CrossSigningManager::deviceTrustToJson(const DeviceTrustResult& result) { return {}; }
bool CrossSigningManager::verifyKeySignatures(const CSM_CrossSigningKey& key) { return {}; }
CrossSigningManager::CrossSigningManager() {}
CrossSigningManager::~CrossSigningManager() {}
CSM_KeyUsage keyUsageFromString(const std::string& s) { return {}; }
DeviceDeletionRequest::DeviceDeletionRequest() {}
DeviceDeletionRequest::~DeviceDeletionRequest() {}
DeviceRenameRequest::DeviceRenameRequest() {}
DeviceRenameRequest::~DeviceRenameRequest() {}
DeviceSortMode::DeviceSortMode() {}
DeviceSortMode::~DeviceSortMode() {}
DeviceFilter::DeviceFilter() {}
DeviceFilter::~DeviceFilter() {}
PollOptionFull::PollOptionFull() {}
PollOptionFull::~PollOptionFull() {}
PollVote::PollVote() {}
PollVote::~PollVote() {}
PollEnd::PollEnd() {}
PollEnd::~PollEnd() {}
PollResultFull::PollResultFull() {}
PollResultFull::~PollResultFull() {}
PollEventDisplay::PollEventDisplay() {}
PollEventDisplay::~PollEventDisplay() {}
string PollManager::buildPollStartContent(const std::string& question, const std::vector<std::string>& optionTexts, PollKind kind, int maxSelections, bool unstable, std::string& error) { return {}; }
PollContent PollManager::parsePollStartContent(const std::string& contentJson, bool unstable) { return {}; }
bool PollManager::isValidPollQuestion(const std::string& question) { return {}; }
bool PollManager::isValidPollOption(const std::string& text) { return {}; }
bool PollManager::isValidMaxSelections(int selections, int optionCount) { return {}; }
string PollManager::buildPollResponseContent(const std::string& pollId, const std::vector<std::string>& selectedOptionIds, bool unstable) { return {}; }
PollVote PollManager::parsePollResponseContent(const std::string& contentJson, const std::string& voterId, const std::string& voterName, bool unstable) { return {}; }
string PollManager::buildPollEndContent(const std::string& pollId, const std::string& reason, bool unstable) { return {}; }
PollEnd PollManager::parsePollEndContent(const std::string& contentJson, bool unstable) { return {}; }
PollResultFull PollManager::tallyVotes(const PollContent& poll, const std::vector<PollVote>& votes) { return {}; }
void PollManager::setMyVote(PollResultFull& result, const std::string& userId) {}
PollEventDisplay PollManager::formatPollEvent(const PollResultFull& result) { return {}; }
string PollManager::formatPollPlainText(const PollEventDisplay& display) { return {}; }
string PollManager::formatPollHtml(const PollEventDisplay& display) { return {}; }
string PollManager::getWinnerText(const PollResultFull& result) { return {}; }
bool PollManager::isPollEvent(const std::string& eventType) { return {}; }
string PollManager::getPollEventDescription(const std::string& eventType) { return {}; }
string PollManager::generatePollId() { return {}; }
string PollManager::optionIdFromIndex(int index) { return {}; }
PollManager::PollManager() {}
PollManager::~PollManager() {}
AliasAvailabilityResult::AliasAvailabilityResult() {}
AliasAvailabilityResult::~AliasAvailabilityResult() {}
RoomDirectoryVisibility visibilityFromString(const std::string& s) { return {}; }
MembershipState::MembershipState() {}
MembershipState::~MembershipState() {}
RoomStateSummary::RoomStateSummary() {}
RoomStateSummary::~RoomStateSummary() {}
void RoomStateManager::setHistoryVisibility(const std::string& roomId, RSM_RoomHistoryVisibility visibility) {}
void RoomStateManager::setJoinRule(const std::string& roomId, RoomJoinRule rule) {}
void RoomStateManager::setRoomName(const std::string& roomId, const std::string& name) {}
void RoomStateManager::setEncrypted(const std::string& roomId, bool encrypted) {}
void RoomStateManager::setMemberCount(const std::string& roomId, int count) {}
RoomStateSummary RoomStateManager::getRoomState(const std::string& roomId) { return {}; }
bool RoomStateManager::canShareRoomHistory(const std::string& roomId) { return {}; }
bool RoomStateManager::isPublicRoom(const std::string& roomId) { return {}; }
bool RoomStateManager::isWorldReadable(const std::string& roomId) { return {}; }
bool RoomStateManager::isInviteOnly(const std::string& roomId) { return {}; }
bool RoomStateManager::areGuestsAllowed(const std::string& roomId) { return {}; }
string RoomStateManager::roomStateToJson(const RoomStateSummary& state) { return {}; }
void RoomStateManager::clear() {}
RoomStateManager::RoomStateManager() {}
RoomStateManager::~RoomStateManager() {}
bool shouldShareHistory(RSM_RoomHistoryVisibility visibility) { return {}; }
bool canSeeEvent(RSM_RoomHistoryVisibility visibility, MembershipState memberStateAtEventTime, MembershipState memberCurrentState) { return {}; }
bool canNonMemberSeeEvents(RSM_RoomHistoryVisibility visibility) { return {}; }
ResourceLimitType::ResourceLimitType() {}
ResourceLimitType::~ResourceLimitType() {}
ResourceLimitMode::ResourceLimitMode() {}
ResourceLimitMode::~ResourceLimitMode() {}
ServerNoticeInfo::ServerNoticeInfo() {}
ServerNoticeInfo::~ServerNoticeInfo() {}
MatrixErrorCodes::MatrixErrorCodes() {}
MatrixErrorCodes::~MatrixErrorCodes() {}
ServerNoticeInfo ServerNoticeManager::parseMatrixError(const std::string& errorJson) { return {}; }
ServerNoticeInfo ServerNoticeManager::parseServerNoticeContent(const std::string& contentJson) { return {}; }
bool ServerNoticeManager::isServerNoticeRoom(const std::string& roomTagsJson) { return {}; }
bool ServerNoticeManager::isServerNoticeTag(const std::string& tagName) { return {}; }
string ServerNoticeManager::formatResourceLimitError(const ServerNoticeInfo& info, ResourceLimitMode mode) { return {}; }
string ServerNoticeManager::formatAdminContactLink(const std::string& adminUri) { return {}; }
string ServerNoticeManager::formatAdminContactText(const std::string& adminUri) { return {}; }
string ServerNoticeManager::formatConsentRequired(const ServerNoticeInfo& info) { return {}; }
string ServerNoticeManager::formatRateLimitMessage(const ServerNoticeInfo& info) { return {}; }
string ServerNoticeManager::getErrorCodeDescription(const std::string& errorCode) { return {}; }
bool ServerNoticeManager::isResourceLimitError(const std::string& errorCode) { return {}; }
bool ServerNoticeManager::isRateLimitError(const std::string& errorCode) { return {}; }
bool ServerNoticeManager::isConsentError(const std::string& errorCode) { return {}; }
bool ServerNoticeManager::isLogoutError(const std::string& errorCode) { return {}; }
bool ServerNoticeManager::isUserDeactivatedError(const std::string& errorCode) { return {}; }
string ServerNoticeManager::getBannerColor(const ServerNoticeInfo& info) { return {}; }
string ServerNoticeManager::formatDowntime(int64_t retryAfterMs) { return {}; }
string ServerNoticeManager::formatServerNotice(const ServerNoticeInfo& info) { return {}; }
string ServerNoticeManager::serverNoticeToJson(const ServerNoticeInfo& info) { return {}; }
string ServerNoticeManager::resourceLimitToJson(const ServerNoticeInfo& info) { return {}; }
string ServerNoticeManager::extractStr(const std::string& json, const std::string& key) { return {}; }
int64_t ServerNoticeManager::extractInt(const std::string& json, const std::string& key) { return {}; }
bool ServerNoticeManager::extractBool(const std::string& json, const std::string& key) { return {}; }
ServerNoticeManager::ServerNoticeManager() {}
ServerNoticeManager::~ServerNoticeManager() {}
ResourceLimitType resourceLimitTypeFromString(const std::string& s) { return {}; }
SpaceNodeType::SpaceNodeType() {}
SpaceNodeType::~SpaceNodeType() {}
SpaceNode::SpaceNode() {}
SpaceNode::~SpaceNode() {}
SpaceChildEntry::SpaceChildEntry() {}
SpaceChildEntry::~SpaceChildEntry() {}
SpaceParentEntry::SpaceParentEntry() {}
SpaceParentEntry::~SpaceParentEntry() {}
SpaceOrderEntry::SpaceOrderEntry() {}
SpaceOrderEntry::~SpaceOrderEntry() {}
SpaceTraversal::SpaceTraversal() {}
SpaceTraversal::~SpaceTraversal() {}
SpaceTraversalOptions::SpaceTraversalOptions() {}
SpaceTraversalOptions::~SpaceTraversalOptions() {}
SpaceGraphResult::SpaceGraphResult() {}
SpaceGraphResult::~SpaceGraphResult() {}
SpaceChildEntry parseSpaceChild(const std::string& stateKey, const std::string& contentJson) { return {}; }
SpaceParentEntry parseSpaceParent(const std::string& contentJson) { return {}; }
LoginFlowType::LoginFlowType() {}
LoginFlowType::~LoginFlowType() {}
SsoProvider::SsoProvider() {}
SsoProvider::~SsoProvider() {}
LoginAuthFlow::LoginAuthFlow() {}
LoginAuthFlow::~LoginAuthFlow() {}
LoginAuthFlowsResult::LoginAuthFlowsResult() {}
LoginAuthFlowsResult::~LoginAuthFlowsResult() {}
LoginAuthFlowsResult parseLoginFlows(const std::string& json) { return {}; }
bool requiresIdentityServer(LoginFlowType type) { return {}; }
MessageType::MessageType() {}
MessageType::~MessageType() {}
MessageContent::MessageContent() {}
MessageContent::~MessageContent() {}
bool isMxcUri(const std::string& url) { return {}; }
MessageType parseMessageType(const std::string& contentJson) { return {}; }
MessageContent parseMessageContent(const std::string& contentJson) { return {}; }
bool supportsThumbnails(MessageType type) { return {}; }
bool isInlineDisplayable(MessageType type) { return {}; }
bool isMediaType(MessageType type) { return {}; }
bool hasTextWithImage(const std::string& contentJson) { return {}; }
bool isMimeTypeImage(const std::string& mt) { return {}; }
bool isMimeTypeVideo(const std::string& mt) { return {}; }
bool isMimeTypeAudio(const std::string& mt) { return {}; }
bool isMimeTypeText(const std::string& mt) { return {}; }
bool isReplyEvent(const std::string& contentJson) { return {}; }
bool isEditionEvent(const std::string& contentJson) { return {}; }

} // namespace progressive