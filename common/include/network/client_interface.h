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

  /**
   * @brief Called when the Photon client emits a debug message.
   * @param debug_level The severity level of the debug message.
   * @param string The debug message text.
   */
  virtual void OnDebugReturn(
      int debug_level, const ExitGames::Common::JString& string) = 0;

  /**
   * @brief Called when a connection-level error occurs.
   * @param error_code The error code identifying the connection failure.
   */
  virtual void OnConnectionError(int error_code) = 0;

  /**
   * @brief Called when a client-level error occurs.
   * @param error_code The error code identifying the client error.
   */
  virtual void OnClientError(int error_code) = 0;

  /**
   * @brief Called when the Photon client issues a warning.
   * @param warning_code The warning code.
   */
  virtual void OnWarning(int warning_code) = 0;

  /**
   * @brief Called when a server-side error occurs.
   * @param error_code The error code identifying the server error.
   */
  virtual void OnServerError(int error_code) = 0;

  /**
   * @brief Called when a player joins the current room.
   * @param player_nr The player number of the joining player.
   * @param player_nrs The list of all player numbers currently in the room.
   * @param player The player who joined the room.
   */
  virtual void OnJoinRoomEvent(
      int player_nr,
      const ExitGames::Common::JVector<int>& player_nrs,
      const ExitGames::LoadBalancing::Player& player) = 0;

  /**
   * @brief Called when a player leaves the current room.
   * @param player_nr The player number of the leaving player.
   * @param is_inactive True if the player is only temporarily inactive.
   */
  virtual void OnLeaveRoomEvent(int player_nr, bool is_inactive) = 0;

  /**
   * @brief Called when a custom event is received from another player.
   * @param player_nr The player number of the sender.
   * @param event_code The application-defined event code.
   * @param event_content The event payload data.
   */
  virtual void OnCustomEvent(
      int player_nr, nByte event_code,
      const ExitGames::Common::Object& event_content) = 0;

  /**
   * @brief Called when the client finishes connecting to the server.
   * @param error_code The error code (0 on success).
   * @param error_string A human-readable error description.
   * @param region The connected region.
   * @param cluster The connected cluster.
   */
  virtual void OnConnect(
      int error_code, const ExitGames::Common::JString& error_string,
      const ExitGames::Common::JString& region,
      const ExitGames::Common::JString& cluster) = 0;

  /**
   * @brief Called when the client disconnects from the server.
   */
  virtual void OnDisconnect() = 0;

  /**
   * @brief Called when the client finishes leaving a room.
   * @param error_code The error code (0 on success).
   * @param error_string A human-readable error description.
   */
  virtual void OnLeaveRoom(
      int error_code, const ExitGames::Common::JString& error_string) = 0;

  // --- Optional (default empty, override as needed) ---

  /**
   * @brief Called when a room has been created.
   * @param local_player_nr The player number assigned to the local player.
   * @param room_properties The initial room properties.
   * @param player_properties The initial player properties.
   * @param error_code The error code (0 on success).
   * @param error_string A human-readable error description.
   */
  virtual void OnCreateRoom(
      int /*local_player_nr*/,
      const ExitGames::Common::Hashtable& /*room_properties*/,
      const ExitGames::Common::Hashtable& /*player_properties*/,
      int /*error_code*/, const ExitGames::Common::JString& /*error_string*/) {}

  /**
   * @brief Called when a join-or-create room operation completes.
   * @param local_player_nr The player number assigned to the local player.
   * @param room_properties The room properties.
   * @param player_properties The player properties.
   * @param error_code The error code (0 on success).
   * @param error_string A human-readable error description.
   */
  virtual void OnJoinOrCreateRoom(
      int /*local_player_nr*/,
      const ExitGames::Common::Hashtable& /*room_properties*/,
      const ExitGames::Common::Hashtable& /*player_properties*/,
      int /*error_code*/, const ExitGames::Common::JString& /*error_string*/) {}

  /**
   * @brief Called when a join-random-or-create room operation completes.
   * @param local_player_nr The player number assigned to the local player.
   * @param room_properties The room properties.
   * @param player_properties The player properties.
   * @param error_code The error code (0 on success).
   * @param error_string A human-readable error description.
   */
  virtual void OnJoinRandomOrCreateRoom(
      int /*local_player_nr*/,
      const ExitGames::Common::Hashtable& /*room_properties*/,
      const ExitGames::Common::Hashtable& /*player_properties*/,
      int /*error_code*/, const ExitGames::Common::JString& /*error_string*/) {}

  /**
   * @brief Called when a join room operation completes.
   * @param local_player_nr The player number assigned to the local player.
   * @param room_properties The room properties.
   * @param player_properties The player properties.
   * @param error_code The error code (0 on success).
   * @param error_string A human-readable error description.
   */
  virtual void OnJoinRoom(
      int /*local_player_nr*/,
      const ExitGames::Common::Hashtable& /*room_properties*/,
      const ExitGames::Common::Hashtable& /*player_properties*/,
      int /*error_code*/, const ExitGames::Common::JString& /*error_string*/) {}

  /**
   * @brief Called when a join-random-room operation completes.
   * @param local_player_nr The player number assigned to the local player.
   * @param room_properties The room properties.
   * @param player_properties The player properties.
   * @param error_code The error code (0 on success).
   * @param error_string A human-readable error description.
   */
  virtual void OnJoinRandomRoom(
      int /*local_player_nr*/,
      const ExitGames::Common::Hashtable& /*room_properties*/,
      const ExitGames::Common::Hashtable& /*player_properties*/,
      int /*error_code*/, const ExitGames::Common::JString& /*error_string*/) {}

  /**
   * @brief Called when the client joins a lobby.
   */
  virtual void OnJoinLobby() {}

  /**
   * @brief Called when the client leaves a lobby.
   */
  virtual void OnLeaveLobby() {}

  /**
   * @brief Called when a find-friends request completes.
   */
  virtual void OnFindFriendsResponse() {}

  /**
   * @brief Called when lobby statistics are received in response to a request.
   * @param lobby_stats The list of lobby statistics.
   */
  virtual void OnLobbyStatsResponse(
      const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& /*lobby_stats*/) {}

  /**
   * @brief Called when a WebRPC call completes.
   * @param error_code The error code (0 on success).
   * @param error_string A human-readable error description.
   * @param uri_path The URI path of the WebRPC endpoint.
   * @param result_code The application-level result code.
   * @param return_data The returned key-value data.
   */
  virtual void OnWebRpc(
      int /*error_code*/, const ExitGames::Common::JString& /*error_string*/,
      const ExitGames::Common::JString& /*uri_path*/, int /*result_code*/,
      const ExitGames::Common::Dictionary<ExitGames::Common::Object, ExitGames::Common::Object>& /*return_data*/) {}

  /**
   * @brief Called when a properties change operation fails.
   */
  virtual void OnPropertiesChangeFailed() {}

  /**
   * @brief Called when the room list is updated.
   */
  virtual void OnRoomListUpdate() {}

  /**
   * @brief Called when room properties change.
   * @param changes The properties that changed.
   */
  virtual void OnRoomPropertiesChange(
      const ExitGames::Common::Hashtable& /*changes*/) {}

  /**
   * @brief Called when a player's properties change.
   * @param player_nr The player number whose properties changed.
   * @param changes The properties that changed.
   */
  virtual void OnPlayerPropertiesChange(
      int /*player_nr*/, const ExitGames::Common::Hashtable& /*changes*/) {}

  /**
   * @brief Called when application-level statistics are updated.
   */
  virtual void OnAppStatsUpdate() {}

  /**
   * @brief Called when lobby statistics are updated via a push notification.
   * @param lobby_stats The updated lobby statistics.
   */
  virtual void OnLobbyStatsUpdate(
      const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& /*lobby_stats*/) {}

  /**
   * @brief Called when the active cache slice changes.
   * @param cache_slice_index The index of the new cache slice.
   */
  virtual void OnCacheSliceChanged(int /*cache_slice_index*/) {}

  /**
   * @brief Called when the master client changes in the current room.
   * @param id The player number of the new master client.
   * @param old_id The player number of the previous master client.
   */
  virtual void OnMasterClientChanged(int /*id*/, int /*old_id*/) {}

  /**
   * @brief Called during a custom authentication flow as an intermediate step.
   * @param custom_response_data The custom authentication response data.
   */
  virtual void OnCustomAuthenticationIntermediateStep(
      const ExitGames::Common::Dictionary<ExitGames::Common::JString, ExitGames::Common::Object>& /*custom_response_data*/) {}

  /**
   * @brief Called when the list of available regions is received.
   * @param available_regions The names of the available regions.
   * @param available_region_servers The server addresses of the available regions.
   */
  virtual void OnAvailableRegions(
      const ExitGames::Common::JVector<ExitGames::Common::JString>& /*available_regions*/,
      const ExitGames::Common::JVector<ExitGames::Common::JString>& /*available_region_servers*/) {}

  /**
   * @brief Called when the authentication secret is received from the server.
   * @param secret The authentication secret.
   */
  virtual void OnSecretReceival(
      const ExitGames::Common::JString& /*secret*/) {}

  /**
   * @brief Called when a direct connection to a remote player is established.
   * @param remote_id The player number of the remote player.
   */
  virtual void OnDirectConnectionEstablished(int /*remote_id*/) {}

  /**
   * @brief Called when a direct connection to a remote player fails.
   * @param remote_id The player number of the remote player.
   */
  virtual void OnDirectConnectionFailedToEstablish(int /*remote_id*/) {}

  /**
   * @brief Called when a direct message is received from a remote player.
   * @param msg The message payload.
   * @param remote_id The player number of the sender.
   * @param relay True if the message was relayed through the server.
   */
  virtual void OnDirectMessage(
      const ExitGames::Common::Object& /*msg*/, int /*remote_id*/, bool /*relay*/) {}

  /**
   * @brief Called when a custom operation response is received.
   * @param operation_response The operation response from the server.
   */
  virtual void OnCustomOperationResponse(
      const ExitGames::Photon::OperationResponse& /*operation_response*/) {}

  /**
   * @brief Called when a get-room-list response is received.
   * @param room_list The list of rooms.
   * @param room_name_list The list of room names.
   */
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
