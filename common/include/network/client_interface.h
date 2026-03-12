#ifndef GPR924_ENGINE_CLIENT_INTERFACE_H
#define GPR924_ENGINE_CLIENT_INTERFACE_H

#ifdef ENABLE_NETWORK

#include "third_party/photon_include.h"

namespace common {
class ClientInterface : public ExitGames::LoadBalancing::Listener {
  public:
};
}

#endif

#endif  // GPR924_ENGINE_CLIENT_INTERFACE_H
