#ifndef GPR924_ENGINE_ENGINE_H
#define GPR924_ENGINE_ENGINE_H

#include "engine/system.h"
#include "utils/observer.h"

namespace common {



using SystemObserverSubject = ObserverSubject<SystemInterface>;

void RunEngine();
float GetDeltaTime();
}

#endif  // GPR924_ENGINE_ENGINE_H
