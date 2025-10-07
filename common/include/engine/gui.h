#ifndef GPR924_ENGINE_GUI_RENDERER_H
#define GPR924_ENGINE_GUI_RENDERER_H

#include "third_party/sdl3_include.h"
#include "utils/observer.h"

namespace common {

class OnGuiInterface
{
public:
  virtual ~OnGuiInterface() = default;

  virtual void OnGui() = 0;
};

using OnGuiObserverSubject = ObserverSubject<OnGuiInterface>;

void BeginGuiRenderer();

void UpdateGuiRenderer();

void EndGuiRenderer();

void DrawGuiRenderer();

void ManageGuiEvent(const SDL_Event &event);
}

#endif  // GPR924_ENGINE_GUI_RENDERER_H
