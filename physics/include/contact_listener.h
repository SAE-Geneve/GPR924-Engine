//
// Created by hugze on 02.11.2025.
//

#ifndef CONTACT_LISTENER_H
#define CONTACT_LISTENER_H

#include "world.h"

namespace listeners {
class ContactListener {
 public:
  virtual ~ContactListener() = default;

  virtual void OnTriggerEnter(physics::CollidersPair cols) = 0;
  virtual void OnTriggerExit(physics::CollidersPair cols) = 0;

  virtual void OnColliderEnter(physics::CollidersPair cols) = 0;
  virtual void OnColliderExit(physics::CollidersPair cols) = 0;
};
}




#endif //CONTACT_LISTENER_H
