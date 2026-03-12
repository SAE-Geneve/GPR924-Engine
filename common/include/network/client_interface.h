#ifndef GPR924_ENGINE_CLIENT_INTERFACE_H
#define GPR924_ENGINE_CLIENT_INTERFACE_H

#ifdef ENABLE_NETWORK

#include "third_party/photon_include.h"

namespace common {
class ClientInterface : public ExitGames::LoadBalancing::Listener {
 public:
  ~ClientInterface() override = default;

 protected:
  // --- Pure virtual (must be implemented by subclasses) ---
  virtual void OnDebugReturn(
      int debug_level, const ExitGames::Common::JString& string) = 0;
  virtual void OnConnectionError(int error_code) = 0;
  virtual void OnClientError(int error_code) = 0;
  virtual void OnWarning(int warning_code) = 0;
  virtual void OnServerError(int error_code) = 0;
  virtual void OnJoinRoomEvent(
      int player_nr,
      const ExitGames::Common::JVector<int>& player_nrs,
      const ExitGames::LoadBalancing::Player& player) = 0;
  virtual void OnLeaveRoomEvent(int player_nr, bool is_inactive) = 0;
  virtual void OnCustomEvent(
      int player_nr, nByte event_code,
      const ExitGames::Common::Object& event_content) = 0;
  virtual void OnConnect(
      int error_code, const ExitGames::Common::JString& error_string,
      const ExitGames::Common::JString& region,
      const ExitGames::Common::JString& cluster) = 0;
  virtual void OnDisconnect() = 0;
  virtual void OnLeaveRoom(
      int error_code, const ExitGames::Common::JString& error_string) = 0;

  // --- Optional (default empty, override as needed) ---
  virtual void OnCreateRoom(
      int /*local_player_nr*/,
      const ExitGames::Common::Hashtable& /*room_properties*/,
      const ExitGames::Common::Hashtable& /*player_properties*/,
      int /*error_code*/, const ExitGames::Common::JString& /*error_string*/) {}
  virtual void OnJoinOrCreateRoom(
      int /*local_player_nr*/,
      const ExitGames::Common::Hashtable& /*room_properties*/,
      const ExitGames::Common::Hashtable& /*player_properties*/,
      int /*error_code*/, const ExitGames::Common::JString& /*error_string*/) {}
  virtual void OnJoinRandomOrCreateRoom(
      int /*local_player_nr*/,
      const ExitGames::Common::Hashtable& /*room_properties*/,
      const ExitGames::Common::Hashtable& /*player_properties*/,
      int /*error_code*/, const ExitGames::Common::JString& /*error_string*/) {}
  virtual void OnJoinRoom(
      int /*local_player_nr*/,
      const ExitGames::Common::Hashtable& /*room_properties*/,
      const ExitGames::Common::Hashtable& /*player_properties*/,
      int /*error_code*/, const ExitGames::Common::JString& /*error_string*/) {}
  virtual void OnJoinRandomRoom(
      int /*local_player_nr*/,
      const ExitGames::Common::Hashtable& /*room_properties*/,
      const ExitGames::Common::Hashtable& /*player_properties*/,
      int /*error_code*/, const ExitGames::Common::JString& /*error_string*/) {}
  virtual void OnJoinLobby() {}
  virtual void OnLeaveLobby() {}
  virtual void OnFindFriendsResponse() {}
  virtual void OnLobbyStatsResponse(
      const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& /*lobby_stats*/) {}
  virtual void OnWebRpc(
      int /*error_code*/, const ExitGames::Common::JString& /*error_string*/,
      const ExitGames::Common::JString& /*uri_path*/, int /*result_code*/,
      const ExitGames::Common::Dictionary<ExitGames::Common::Object, ExitGames::Common::Object>& /*return_data*/) {}
  virtual void OnPropertiesChangeFailed() {}
  virtual void OnRoomListUpdate() {}
  virtual void OnRoomPropertiesChange(
      const ExitGames::Common::Hashtable& /*changes*/) {}
  virtual void OnPlayerPropertiesChange(
      int /*player_nr*/, const ExitGames::Common::Hashtable& /*changes*/) {}
  virtual void OnAppStatsUpdate() {}
  virtual void OnLobbyStatsUpdate(
      const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& /*lobby_stats*/) {}
  virtual void OnCacheSliceChanged(int /*cache_slice_index*/) {}
  virtual void OnMasterClientChanged(int /*id*/, int /*old_id*/) {}
  virtual void OnCustomAuthenticationIntermediateStep(
      const ExitGames::Common::Dictionary<ExitGames::Common::JString, ExitGames::Common::Object>& /*custom_response_data*/) {}
  virtual void OnAvailableRegions(
      const ExitGames::Common::JVector<ExitGames::Common::JString>& /*available_regions*/,
      const ExitGames::Common::JVector<ExitGames::Common::JString>& /*available_region_servers*/) {}
  virtual void OnSecretReceival(
      const ExitGames::Common::JString& /*secret*/) {}
  virtual void OnDirectConnectionEstablished(int /*remote_id*/) {}
  virtual void OnDirectConnectionFailedToEstablish(int /*remote_id*/) {}
  virtual void OnDirectMessage(
      const ExitGames::Common::Object& /*msg*/, int /*remote_id*/, bool /*relay*/) {}
  virtual void OnCustomOperationResponse(
      const ExitGames::Photon::OperationResponse& /*operation_response*/) {}
  virtual void OnGetRoomListResponse(
      const ExitGames::Common::JVector<ExitGames::Common::Helpers::SharedPointer<ExitGames::LoadBalancing::Room> >& /*room_list*/,
      const ExitGames::Common::JVector<ExitGames::Common::JString>& /*room_name_list*/) {}

 private:
  // --- Photon Listener overrides (delegate to protected virtuals) ---
  void debugReturn(int debugLevel,
                   const ExitGames::Common::JString& string) override {
    OnDebugReturn(debugLevel, string);
  }
  void connectionErrorReturn(int errorCode) override {
    OnConnectionError(errorCode);
  }
  void clientErrorReturn(int errorCode) override {
    OnClientError(errorCode);
  }
  void warningReturn(int warningCode) override {
    OnWarning(warningCode);
  }
  void serverErrorReturn(int errorCode) override {
    OnServerError(errorCode);
  }
  void joinRoomEventAction(
      int playerNr, const ExitGames::Common::JVector<int>& playernrs,
      const ExitGames::LoadBalancing::Player& player) override {
    OnJoinRoomEvent(playerNr, playernrs, player);
  }
  void leaveRoomEventAction(int playerNr, bool isInactive) override {
    OnLeaveRoomEvent(playerNr, isInactive);
  }
  void customEventAction(
      int playerNr, nByte eventCode,
      const ExitGames::Common::Object& eventContent) override {
    OnCustomEvent(playerNr, eventCode, eventContent);
  }
  void connectReturn(int errorCode,
                     const ExitGames::Common::JString& errorString,
                     const ExitGames::Common::JString& region,
                     const ExitGames::Common::JString& cluster) override {
    OnConnect(errorCode, errorString, region, cluster);
  }
  void disconnectReturn() override {
    OnDisconnect();
  }
  void leaveRoomReturn(int errorCode,
                       const ExitGames::Common::JString& errorString) override {
    OnLeaveRoom(errorCode, errorString);
  }
  void createRoomReturn(
      int localPlayerNr,
      const ExitGames::Common::Hashtable& roomProperties,
      const ExitGames::Common::Hashtable& playerProperties,
      int errorCode,
      const ExitGames::Common::JString& errorString) override {
    OnCreateRoom(localPlayerNr, roomProperties, playerProperties, errorCode,
                 errorString);
  }
  void joinOrCreateRoomReturn(
      int localPlayerNr,
      const ExitGames::Common::Hashtable& roomProperties,
      const ExitGames::Common::Hashtable& playerProperties,
      int errorCode,
      const ExitGames::Common::JString& errorString) override {
    OnJoinOrCreateRoom(localPlayerNr, roomProperties, playerProperties,
                       errorCode, errorString);
  }
  void joinRandomOrCreateRoomReturn(
      int localPlayerNr,
      const ExitGames::Common::Hashtable& roomProperties,
      const ExitGames::Common::Hashtable& playerProperties,
      int errorCode,
      const ExitGames::Common::JString& errorString) override {
    OnJoinRandomOrCreateRoom(localPlayerNr, roomProperties, playerProperties,
                             errorCode, errorString);
  }
  void joinRoomReturn(
      int localPlayerNr,
      const ExitGames::Common::Hashtable& roomProperties,
      const ExitGames::Common::Hashtable& playerProperties,
      int errorCode,
      const ExitGames::Common::JString& errorString) override {
    OnJoinRoom(localPlayerNr, roomProperties, playerProperties, errorCode,
               errorString);
  }
  void joinRandomRoomReturn(
      int localPlayerNr,
      const ExitGames::Common::Hashtable& roomProperties,
      const ExitGames::Common::Hashtable& playerProperties,
      int errorCode,
      const ExitGames::Common::JString& errorString) override {
    OnJoinRandomRoom(localPlayerNr, roomProperties, playerProperties,
                     errorCode, errorString);
  }
  void joinLobbyReturn() override {
    OnJoinLobby();
  }
  void leaveLobbyReturn() override {
    OnLeaveLobby();
  }
  void onFindFriendsResponse() override {
    OnFindFriendsResponse();
  }
  void onLobbyStatsResponse(
      const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats) override {
    OnLobbyStatsResponse(lobbyStats);
  }
  void webRpcReturn(
      int errorCode, const ExitGames::Common::JString& errorString,
      const ExitGames::Common::JString& uriPath, int resultCode,
      const ExitGames::Common::Dictionary<ExitGames::Common::Object, ExitGames::Common::Object>& returnData) override {
    OnWebRpc(errorCode, errorString, uriPath, resultCode, returnData);
  }
  void onPropertiesChangeFailed() override {
    OnPropertiesChangeFailed();
  }
  void onRoomListUpdate() override {
    OnRoomListUpdate();
  }
  void onRoomPropertiesChange(
      const ExitGames::Common::Hashtable& changes) override {
    OnRoomPropertiesChange(changes);
  }
  void onPlayerPropertiesChange(
      int playerNr, const ExitGames::Common::Hashtable& changes) override {
    OnPlayerPropertiesChange(playerNr, changes);
  }
  void onAppStatsUpdate() override {
    OnAppStatsUpdate();
  }
  void onLobbyStatsUpdate(
      const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats) override {
    OnLobbyStatsUpdate(lobbyStats);
  }
  void onCacheSliceChanged(int cacheSliceIndex) override {
    OnCacheSliceChanged(cacheSliceIndex);
  }
  void onMasterClientChanged(int id, int oldID) override {
    OnMasterClientChanged(id, oldID);
  }
  void onCustomAuthenticationIntermediateStep(
      const ExitGames::Common::Dictionary<ExitGames::Common::JString, ExitGames::Common::Object>& customResponseData) override {
    OnCustomAuthenticationIntermediateStep(customResponseData);
  }
  void onAvailableRegions(
      const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegions,
      const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegionServers) override {
    OnAvailableRegions(availableRegions, availableRegionServers);
  }
  void onSecretReceival(
      const ExitGames::Common::JString& secret) override {
    OnSecretReceival(secret);
  }
  void onDirectConnectionEstablished(int remoteID) override {
    OnDirectConnectionEstablished(remoteID);
  }
  void onDirectConnectionFailedToEstablish(int remoteID) override {
    OnDirectConnectionFailedToEstablish(remoteID);
  }
  void onDirectMessage(const ExitGames::Common::Object& msg, int remoteID,
                       bool relay) override {
    OnDirectMessage(msg, remoteID, relay);
  }
  void onCustomOperationResponse(
      const ExitGames::Photon::OperationResponse& operationResponse) override {
    OnCustomOperationResponse(operationResponse);
  }
  void onGetRoomListResponse(
      const ExitGames::Common::JVector<ExitGames::Common::Helpers::SharedPointer<ExitGames::LoadBalancing::Room> >& roomList,
      const ExitGames::Common::JVector<ExitGames::Common::JString>& roomNameList) override {
    OnGetRoomListResponse(roomList, roomNameList);
  }
};
}  // namespace common

#endif

#endif  // GPR924_ENGINE_CLIENT_INTERFACE_H
