#include "network/network_manager.h"

#define NEKO_WIDE2(x) L##x
#define NEKO_WIDE(x) NEKO_WIDE2(x)
static const ExitGames::Common::JString appID = NEKO_WIDE(PHOTON_APP_ID);
static const ExitGames::Common::JString appVersion = L"1.0";

namespace common {

void NetworkManager::Begin() {
  ExitGames::LoadBalancing::ConnectOptions connectOptions_{};
  if (!loadBalancingClient_.connect(connectOptions_)) {
    throw std::runtime_error("Failed to connect to the load balancing client");
  }
}
void NetworkManager::End() {loadBalancingClient_.disconnect();}
void NetworkManager::Update([[maybe_unused]]float dt) {
  throw std::runtime_error("Please call Fixed Update");
}
void NetworkManager::FixedUpdate() {
  loadBalancingClient_.service();
}
NetworkManager::NetworkManager(
    ClientInterface* client,
    const ExitGames::LoadBalancing::ClientConstructOptions& options)
    : loadBalancingClient_(*client, appID, appVersion, options) {}
}  // namespace common