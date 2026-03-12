#ifndef GPR924_ENGINE_NETWORK_MANAGER_H
#define GPR924_ENGINE_NETWORK_MANAGER_H

#ifdef ENABLE_NETWORK
#include "engine/system.h"
#include "network/client_interface.h"

namespace common {

class NetworkManager : public SystemInterface {
 public:
  void Begin() override;
  void End() override;
  void Update(float dt) override;
  void FixedUpdate() override;
  NetworkManager(ClientInterface* client, const ExitGames::LoadBalancing::ClientConstructOptions& options = {});
private:
  ExitGames::LoadBalancing::Client loadBalancingClient_;
};

}
#endif
#endif  // GPR924_ENGINE_NETWORK_MANAGER_H
