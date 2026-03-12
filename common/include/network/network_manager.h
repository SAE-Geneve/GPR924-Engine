#ifndef GPR924_ENGINE_NETWORK_MANAGER_H
#define GPR924_ENGINE_NETWORK_MANAGER_H

#ifdef ENABLE_NETWORK
#include "engine/system.h"
#include "network/client_interface.h"

namespace common {

class NetworkManager : public SystemInterface {
 public:
  /**
   * @brief Constructs a NetworkManager with a client listener.
   * @param client The client interface that receives network callbacks.
   * @param options Construction options for the Photon load balancing client.
   */
  NetworkManager(
      ClientInterface* client,
      const ExitGames::LoadBalancing::ClientConstructOptions& options = {});

  /**
   * @brief Connects to the Photon load balancing server.
   */
  void Begin() override;

  /**
   * @brief Disconnects from the Photon server.
   */
  void End() override;

  /**
   * @brief Not supported; throws an exception directing callers to use FixedUpdate.
   * @param dt The delta time (unused).
   */
  void Update(float dt) override;

  /**
   * @brief Services the Photon client. Call this at fixed intervals.
   */
  void FixedUpdate() override;

  /**
   * @brief Returns a reference to the underlying Photon load balancing client.
   * @return A reference to the load balancing client.
   */
  auto& load_balancing_client() { return load_balancing_client_; }

 private:
  ExitGames::LoadBalancing::Client load_balancing_client_;
};

}  // namespace common
#endif
#endif  // GPR924_ENGINE_NETWORK_MANAGER_H
