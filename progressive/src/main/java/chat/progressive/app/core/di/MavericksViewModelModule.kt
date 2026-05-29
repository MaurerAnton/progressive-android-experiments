/*
 * Copyright 2019-2024 Progressive Chat
 *
 * SPDX-License-Identifier: AGPL-3.0-only OR LicenseRef-Progressive
 * Please see LICENSE files in the repository root for full details.
 */

package chat.progressive.app.core.di

import dagger.Binds
import dagger.Module
import dagger.hilt.InstallIn
import dagger.multibindings.IntoMap
import chat.progressive.app.features.analytics.accountdata.AnalyticsAccountDataViewModel
import chat.progressive.app.features.analytics.ui.consent.AnalyticsConsentViewModel
import chat.progressive.app.features.attachments.AttachmentTypeSelectorViewModel
import chat.progressive.app.features.auth.ReAuthViewModel
import chat.progressive.app.features.call.ProgressiveCallViewModel
import chat.progressive.app.features.call.conference.JitsiCallViewModel
import chat.progressive.app.features.call.transfer.CallTransferViewModel
import chat.progressive.app.features.contactsbook.ContactsBookViewModel
import chat.progressive.app.features.createdirect.CreateDirectRoomViewModel
import chat.progressive.app.features.crypto.keysbackup.settings.KeysBackupSettingsViewModel
import chat.progressive.app.features.crypto.quads.SharedSecureStorageViewModel
import chat.progressive.app.features.crypto.recover.BootstrapSharedViewModel
import chat.progressive.app.features.crypto.verification.self.SelfVerificationViewModel
import chat.progressive.app.features.crypto.verification.user.UserVerificationViewModel
import chat.progressive.app.features.devtools.RoomDevToolViewModel
import chat.progressive.app.features.discovery.DiscoverySettingsViewModel
import chat.progressive.app.features.discovery.change.SetIdentityServerViewModel
import chat.progressive.app.features.home.HomeActivityViewModel
import chat.progressive.app.features.home.HomeDetailViewModel
import chat.progressive.app.features.home.NewHomeDetailViewModel
import chat.progressive.app.features.home.UnknownDeviceDetectorSharedViewModel
import chat.progressive.app.features.home.UnreadMessagesSharedViewModel
import chat.progressive.app.features.home.UserColorAccountDataViewModel
import chat.progressive.app.features.home.room.breadcrumbs.BreadcrumbsViewModel
import chat.progressive.app.features.home.room.detail.TimelineViewModel
import chat.progressive.app.features.home.room.detail.composer.MessageComposerViewModel
import chat.progressive.app.features.home.room.detail.composer.link.SetLinkViewModel
import chat.progressive.app.features.home.room.detail.search.SearchViewModel
import chat.progressive.app.features.home.room.detail.timeline.action.MessageActionsViewModel
import chat.progressive.app.features.home.room.detail.timeline.edithistory.ViewEditHistoryViewModel
import chat.progressive.app.features.home.room.detail.timeline.reactions.ViewReactionsViewModel
import chat.progressive.app.features.home.room.detail.upgrade.MigrateRoomViewModel
import chat.progressive.app.features.home.room.list.RoomListViewModel
import chat.progressive.app.features.home.room.list.home.HomeRoomListViewModel
import chat.progressive.app.features.home.room.list.home.invites.InvitesViewModel
import chat.progressive.app.features.home.room.list.home.release.ReleaseNotesViewModel
import chat.progressive.app.features.invite.InviteUsersToRoomViewModel
import chat.progressive.app.features.location.LocationSharingViewModel
import chat.progressive.app.features.location.live.map.LiveLocationMapViewModel
import chat.progressive.app.features.location.preview.LocationPreviewViewModel
import chat.progressive.app.features.login.LoginViewModel
import chat.progressive.app.features.matrixto.MatrixToBottomSheetViewModel
import chat.progressive.app.features.media.ProgressiveAttachmentViewModel
import chat.progressive.app.features.onboarding.OnboardingViewModel
import chat.progressive.app.features.poll.create.CreatePollViewModel
import chat.progressive.app.features.qrcode.QrCodeScannerViewModel
import chat.progressive.app.features.rageshake.BugReportViewModel
import chat.progressive.app.features.reactions.EmojiSearchResultViewModel
import chat.progressive.app.features.room.RequireActiveMembershipViewModel
import chat.progressive.app.features.roomdirectory.RoomDirectoryViewModel
import chat.progressive.app.features.roomdirectory.createroom.CreateRoomViewModel
import chat.progressive.app.features.roomdirectory.picker.RoomDirectoryPickerViewModel
import chat.progressive.app.features.roomdirectory.roompreview.RoomPreviewViewModel
import chat.progressive.app.features.roommemberprofile.RoomMemberProfileViewModel
import chat.progressive.app.features.roommemberprofile.devices.DeviceListBottomSheetViewModel
import chat.progressive.app.features.roomprofile.RoomProfileViewModel
import chat.progressive.app.features.roomprofile.alias.RoomAliasViewModel
import chat.progressive.app.features.roomprofile.alias.detail.RoomAliasBottomSheetViewModel
import chat.progressive.app.features.roomprofile.banned.RoomBannedMemberListViewModel
import chat.progressive.app.features.roomprofile.members.RoomMemberListViewModel
import chat.progressive.app.features.roomprofile.notifications.RoomNotificationSettingsViewModel
import chat.progressive.app.features.roomprofile.permissions.RoomPermissionsViewModel
import chat.progressive.app.features.roomprofile.polls.RoomPollsViewModel
import chat.progressive.app.features.roomprofile.polls.detail.ui.RoomPollDetailViewModel
import chat.progressive.app.features.roomprofile.settings.RoomSettingsViewModel
import chat.progressive.app.features.roomprofile.settings.joinrule.advanced.RoomJoinRuleChooseRestrictedViewModel
import chat.progressive.app.features.roomprofile.uploads.RoomUploadsViewModel
import chat.progressive.app.features.settings.account.deactivation.DeactivateAccountViewModel
import chat.progressive.app.features.settings.crosssigning.CrossSigningSettingsViewModel
import chat.progressive.app.features.settings.devices.DeviceVerificationInfoBottomSheetViewModel
import chat.progressive.app.features.settings.devices.DevicesViewModel
import chat.progressive.app.features.settings.devices.v2.details.SessionDetailsViewModel
import chat.progressive.app.features.settings.devices.v2.more.SessionLearnMoreViewModel
import chat.progressive.app.features.settings.devices.v2.othersessions.OtherSessionsViewModel
import chat.progressive.app.features.settings.devices.v2.overview.SessionOverviewViewModel
import chat.progressive.app.features.settings.devices.v2.rename.RenameSessionViewModel
import chat.progressive.app.features.settings.devtools.AccountDataViewModel
import chat.progressive.app.features.settings.devtools.GossipingEventsPaperTrailViewModel
import chat.progressive.app.features.settings.devtools.KeyRequestListViewModel
import chat.progressive.app.features.settings.devtools.KeyRequestViewModel
import chat.progressive.app.features.settings.font.FontScaleSettingViewModel
import chat.progressive.app.features.settings.homeserver.HomeserverSettingsViewModel
import chat.progressive.app.features.settings.ignored.IgnoredUsersViewModel
import chat.progressive.app.features.settings.labs.ProgressiveSettingsLabsVM
import chat.progressive.app.features.settings.legals.LegalsViewModel
import chat.progressive.app.features.settings.locale.LocalePickerViewModel
import chat.progressive.app.features.settings.notifications.ProgressiveSettingsNotifViewModel
import chat.progressive.app.features.settings.notifications.ProgressiveSettingsPushRuleVM
import chat.progressive.app.features.settings.push.PushGatewaysViewModel
import chat.progressive.app.features.settings.threepids.ThreePidsSettingsViewModel
import chat.progressive.app.features.share.IncomingShareViewModel
import chat.progressive.app.features.signout.soft.SoftLogoutViewModel
import chat.progressive.app.features.spaces.SpaceListViewModel
import chat.progressive.app.features.spaces.SpaceMenuViewModel
import chat.progressive.app.features.spaces.create.CreateSpaceViewModel
import chat.progressive.app.features.spaces.explore.SpaceDirectoryViewModel
import chat.progressive.app.features.spaces.invite.SpaceInviteBottomSheetViewModel
import chat.progressive.app.features.spaces.leave.SpaceLeaveAdvancedViewModel
import chat.progressive.app.features.spaces.manage.SpaceAddRoomsViewModel
import chat.progressive.app.features.spaces.manage.SpaceManageRoomsViewModel
import chat.progressive.app.features.spaces.manage.SpaceManageSharedViewModel
import chat.progressive.app.features.spaces.people.SpacePeopleViewModel
import chat.progressive.app.features.spaces.preview.SpacePreviewViewModel
import chat.progressive.app.features.spaces.share.ShareSpaceViewModel
import chat.progressive.app.features.start.StartAppViewModel
import chat.progressive.app.features.terms.ReviewTermsViewModel
import chat.progressive.app.features.usercode.UserCodeSharedViewModel
import chat.progressive.app.features.userdirectory.UserListViewModel
import chat.progressive.app.features.widgets.WidgetViewModel
import chat.progressive.app.features.widgets.permissions.RoomWidgetPermissionViewModel
import chat.progressive.app.features.workers.signout.ServerBackupStatusViewModel
import chat.progressive.app.features.workers.signout.SignoutCheckViewModel

@InstallIn(MavericksViewModelComponent::class)
@Module
interface MavericksViewModelModule {

    @Binds
    @IntoMap
    @MavericksViewModelKey(RoomListViewModel::class)
    fun roomListViewModelFactory(factory: RoomListViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(SpaceManageRoomsViewModel::class)
    fun spaceManageRoomsViewModelFactory(factory: SpaceManageRoomsViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(SpaceManageSharedViewModel::class)
    fun spaceManageSharedViewModelFactory(factory: SpaceManageSharedViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(SpaceListViewModel::class)
    fun spaceListViewModelFactory(factory: SpaceListViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(ReAuthViewModel::class)
    fun reAuthViewModelFactory(factory: ReAuthViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(ProgressiveCallViewModel::class)
    fun vectorCallViewModelFactory(factory: ProgressiveCallViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(JitsiCallViewModel::class)
    fun jitsiCallViewModelFactory(factory: JitsiCallViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(RoomDirectoryViewModel::class)
    fun roomDirectoryViewModelFactory(factory: RoomDirectoryViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(ViewReactionsViewModel::class)
    fun viewReactionsViewModelFactory(factory: ViewReactionsViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(RoomWidgetPermissionViewModel::class)
    fun roomWidgetPermissionViewModelFactory(factory: RoomWidgetPermissionViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(WidgetViewModel::class)
    fun widgetViewModelFactory(factory: WidgetViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(ServerBackupStatusViewModel::class)
    fun serverBackupStatusViewModelFactory(factory: ServerBackupStatusViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(SignoutCheckViewModel::class)
    fun signoutCheckViewModelFactory(factory: SignoutCheckViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(RoomDirectoryPickerViewModel::class)
    fun roomDirectoryPickerViewModelFactory(factory: RoomDirectoryPickerViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(RoomDevToolViewModel::class)
    fun roomDevToolViewModelFactory(factory: RoomDevToolViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(MigrateRoomViewModel::class)
    fun migrateRoomViewModelFactory(factory: MigrateRoomViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(IgnoredUsersViewModel::class)
    fun ignoredUsersViewModelFactory(factory: IgnoredUsersViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(CallTransferViewModel::class)
    fun callTransferViewModelFactory(factory: CallTransferViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(ContactsBookViewModel::class)
    fun contactsBookViewModelFactory(factory: ContactsBookViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(CreateDirectRoomViewModel::class)
    fun createDirectRoomViewModelFactory(factory: CreateDirectRoomViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(QrCodeScannerViewModel::class)
    fun qrCodeViewModelFactory(factory: QrCodeScannerViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(RoomNotificationSettingsViewModel::class)
    fun roomNotificationSettingsViewModelFactory(factory: RoomNotificationSettingsViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(KeysBackupSettingsViewModel::class)
    fun keysBackupSettingsViewModelFactory(factory: KeysBackupSettingsViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(SharedSecureStorageViewModel::class)
    fun sharedSecureStorageViewModelFactory(factory: SharedSecureStorageViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(UserListViewModel::class)
    fun userListViewModelFactory(factory: UserListViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(UserCodeSharedViewModel::class)
    fun userCodeSharedViewModelFactory(factory: UserCodeSharedViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(ReviewTermsViewModel::class)
    fun reviewTermsViewModelFactory(factory: ReviewTermsViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(ShareSpaceViewModel::class)
    fun shareSpaceViewModelFactory(factory: ShareSpaceViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(SpacePreviewViewModel::class)
    fun spacePreviewViewModelFactory(factory: SpacePreviewViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(SpacePeopleViewModel::class)
    fun spacePeopleViewModelFactory(factory: SpacePeopleViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(SpaceAddRoomsViewModel::class)
    fun spaceAddRoomsViewModelFactory(factory: SpaceAddRoomsViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(SpaceLeaveAdvancedViewModel::class)
    fun spaceLeaveAdvancedViewModelFactory(factory: SpaceLeaveAdvancedViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(SpaceInviteBottomSheetViewModel::class)
    fun spaceInviteBottomSheetViewModelFactory(factory: SpaceInviteBottomSheetViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(SpaceDirectoryViewModel::class)
    fun spaceDirectoryViewModelFactory(factory: SpaceDirectoryViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(CreateSpaceViewModel::class)
    fun createSpaceViewModelFactory(factory: CreateSpaceViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(SpaceMenuViewModel::class)
    fun spaceMenuViewModelFactory(factory: SpaceMenuViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(SoftLogoutViewModel::class)
    fun softLogoutViewModelFactory(factory: SoftLogoutViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(IncomingShareViewModel::class)
    fun incomingShareViewModelFactory(factory: IncomingShareViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(ThreePidsSettingsViewModel::class)
    fun threePidsSettingsViewModelFactory(factory: ThreePidsSettingsViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(PushGatewaysViewModel::class)
    fun pushGatewaysViewModelFactory(factory: PushGatewaysViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(HomeserverSettingsViewModel::class)
    fun homeserverSettingsViewModelFactory(factory: HomeserverSettingsViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(LocalePickerViewModel::class)
    fun localePickerViewModelFactory(factory: LocalePickerViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(GossipingEventsPaperTrailViewModel::class)
    fun gossipingEventsPaperTrailViewModelFactory(factory: GossipingEventsPaperTrailViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(AccountDataViewModel::class)
    fun accountDataViewModelFactory(factory: AccountDataViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(DevicesViewModel::class)
    fun devicesViewModelFactory(factory: DevicesViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(chat.progressive.app.features.settings.devices.v2.DevicesViewModel::class)
    fun devicesViewModelV2Factory(factory: chat.progressive.app.features.settings.devices.v2.DevicesViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(KeyRequestListViewModel::class)
    fun keyRequestListViewModelFactory(factory: KeyRequestListViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(KeyRequestViewModel::class)
    fun keyRequestViewModelFactory(factory: KeyRequestViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(CrossSigningSettingsViewModel::class)
    fun crossSigningSettingsViewModelFactory(factory: CrossSigningSettingsViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(DeactivateAccountViewModel::class)
    fun deactivateAccountViewModelFactory(factory: DeactivateAccountViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(RoomUploadsViewModel::class)
    fun roomUploadsViewModelFactory(factory: RoomUploadsViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(RoomJoinRuleChooseRestrictedViewModel::class)
    fun roomJoinRuleChooseRestrictedViewModelFactory(factory: RoomJoinRuleChooseRestrictedViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(RoomSettingsViewModel::class)
    fun roomSettingsViewModelFactory(factory: RoomSettingsViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(RoomPermissionsViewModel::class)
    fun roomPermissionsViewModelFactory(factory: RoomPermissionsViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(RoomMemberListViewModel::class)
    fun roomMemberListViewModelFactory(factory: RoomMemberListViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(RoomBannedMemberListViewModel::class)
    fun roomBannedMemberListViewModelFactory(factory: RoomBannedMemberListViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(RoomAliasViewModel::class)
    fun roomAliasViewModelFactory(factory: RoomAliasViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(RoomAliasBottomSheetViewModel::class)
    fun roomAliasBottomSheetViewModelFactory(factory: RoomAliasBottomSheetViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(RoomProfileViewModel::class)
    fun roomProfileViewModelFactory(factory: RoomProfileViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(RoomMemberProfileViewModel::class)
    fun roomMemberProfileViewModelFactory(factory: RoomMemberProfileViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(UserColorAccountDataViewModel::class)
    fun userColorAccountDataViewModelFactory(factory: UserColorAccountDataViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(RoomPreviewViewModel::class)
    fun roomPreviewViewModelFactory(factory: RoomPreviewViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(CreateRoomViewModel::class)
    fun createRoomViewModelFactory(factory: CreateRoomViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(RequireActiveMembershipViewModel::class)
    fun requireActiveMembershipViewModelFactory(factory: RequireActiveMembershipViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(EmojiSearchResultViewModel::class)
    fun emojiSearchResultViewModelFactory(factory: EmojiSearchResultViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(BugReportViewModel::class)
    fun bugReportViewModelFactory(factory: BugReportViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(MatrixToBottomSheetViewModel::class)
    fun matrixToBottomSheetViewModelFactory(factory: MatrixToBottomSheetViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(OnboardingViewModel::class)
    fun onboardingViewModelFactory(factory: OnboardingViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(LoginViewModel::class)
    fun loginViewModelFactory(factory: LoginViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(AnalyticsConsentViewModel::class)
    fun analyticsConsentViewModelFactory(factory: AnalyticsConsentViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(AnalyticsAccountDataViewModel::class)
    fun analyticsAccountDataViewModelFactory(factory: AnalyticsAccountDataViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(StartAppViewModel::class)
    fun startAppViewModelFactory(factory: StartAppViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(InviteUsersToRoomViewModel::class)
    fun inviteUsersToRoomViewModelFactory(factory: InviteUsersToRoomViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(ViewEditHistoryViewModel::class)
    fun viewEditHistoryViewModelFactory(factory: ViewEditHistoryViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(MessageActionsViewModel::class)
    fun messageActionsViewModelFactory(factory: MessageActionsViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(SearchViewModel::class)
    fun searchViewModelFactory(factory: SearchViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(UnreadMessagesSharedViewModel::class)
    fun unreadMessagesSharedViewModelFactory(factory: UnreadMessagesSharedViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(UnknownDeviceDetectorSharedViewModel::class)
    fun unknownDeviceDetectorSharedViewModelFactory(factory: UnknownDeviceDetectorSharedViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(DiscoverySettingsViewModel::class)
    fun discoverySettingsViewModelFactory(factory: DiscoverySettingsViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(LegalsViewModel::class)
    fun legalsViewModelFactory(factory: LegalsViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(TimelineViewModel::class)
    fun roomDetailViewModelFactory(factory: TimelineViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(MessageComposerViewModel::class)
    fun messageComposerViewModelFactory(factory: MessageComposerViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(SetIdentityServerViewModel::class)
    fun setIdentityServerViewModelFactory(factory: SetIdentityServerViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(BreadcrumbsViewModel::class)
    fun breadcrumbsViewModelFactory(factory: BreadcrumbsViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(HomeDetailViewModel::class)
    fun homeDetailViewModelFactory(factory: HomeDetailViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(DeviceVerificationInfoBottomSheetViewModel::class)
    fun deviceVerificationInfoBottomSheetViewModelFactory(factory: DeviceVerificationInfoBottomSheetViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(DeviceListBottomSheetViewModel::class)
    fun deviceListBottomSheetViewModelFactory(factory: DeviceListBottomSheetViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(HomeActivityViewModel::class)
    fun homeActivityViewModelFactory(factory: HomeActivityViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(BootstrapSharedViewModel::class)
    fun bootstrapSharedViewModelFactory(factory: BootstrapSharedViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(UserVerificationViewModel::class)
    fun userVerificationBottomSheetViewModelFactory(factory: UserVerificationViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(SelfVerificationViewModel::class)
    fun selfVerificationBottomSheetViewModelFactory(factory: SelfVerificationViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(CreatePollViewModel::class)
    fun createPollViewModelFactory(factory: CreatePollViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(LocationSharingViewModel::class)
    fun createLocationSharingViewModelFactory(factory: LocationSharingViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(LocationPreviewViewModel::class)
    fun createLocationPreviewViewModelFactory(factory: LocationPreviewViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(ProgressiveAttachmentViewModel::class)
    fun vectorAttachmentViewerViewModelFactory(factory: ProgressiveAttachmentViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(LiveLocationMapViewModel::class)
    fun liveLocationMapViewModelFactory(factory: LiveLocationMapViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(FontScaleSettingViewModel::class)
    fun fontScaleSettingViewModelFactory(factory: FontScaleSettingViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(HomeRoomListViewModel::class)
    fun homeRoomListViewModel(factory: HomeRoomListViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(InvitesViewModel::class)
    fun invitesViewModel(factory: InvitesViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(ReleaseNotesViewModel::class)
    fun releaseNotesViewModel(factory: ReleaseNotesViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(SessionOverviewViewModel::class)
    fun sessionOverviewViewModelFactory(factory: SessionOverviewViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(OtherSessionsViewModel::class)
    fun otherSessionsViewModelFactory(factory: OtherSessionsViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(SessionDetailsViewModel::class)
    fun sessionDetailsViewModelFactory(factory: SessionDetailsViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(RenameSessionViewModel::class)
    fun renameSessionViewModelFactory(factory: RenameSessionViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(SessionLearnMoreViewModel::class)
    fun sessionLearnMoreViewModelFactory(factory: SessionLearnMoreViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(ProgressiveSettingsLabsVM::class)
    fun vectorSettingsLabsViewModelFactory(factory: ProgressiveSettingsLabsVM.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(AttachmentTypeSelectorViewModel::class)
    fun attachmentTypeSelectorViewModelFactory(factory: AttachmentTypeSelectorViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(ProgressiveSettingsNotifViewModel::class)
    fun vectorSettingsNotificationPreferenceViewModelFactory(
            factory: ProgressiveSettingsNotifViewModel.Factory
    ): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(ProgressiveSettingsPushRuleVM::class)
    fun vectorSettingsPushRuleNotificationPreferenceViewModelFactory(
            factory: ProgressiveSettingsPushRuleVM.Factory
    ): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(SetLinkViewModel::class)
    fun setLinkViewModelFactory(factory: SetLinkViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(RoomPollsViewModel::class)
    fun roomPollsViewModelFactory(factory: RoomPollsViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(RoomPollDetailViewModel::class)
    fun roomPollDetailViewModelFactory(factory: RoomPollDetailViewModel.Factory): MavericksAssistedViewModelFactory<*, *>

    @Binds
    @IntoMap
    @MavericksViewModelKey(NewHomeDetailViewModel::class)
    fun newHomeDetailViewModelFactory(factory: NewHomeDetailViewModel.Factory): MavericksAssistedViewModelFactory<*, *>
}
