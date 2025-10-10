#ifndef GPR924_ENGINE_RENDERER_H
#define GPR924_ENGINE_RENDERER_H

#include "third_party/sdl3_include.h"
#include "utils/observer.h"

namespace common {
class DrawInterface
{
public:
  virtual ~DrawInterface() = default;
  virtual void PreDraw(){}
  virtual void Draw() = 0;
  virtual void PostDraw(){}
};

using DrawObserverSubject = ObserverSubject<DrawInterface>;

void BeginRenderer();
void UpdateRenderer();
void DrawRenderer();
void EndRenderer();
void DrawCircle(float centerX, float centerY, float radius,
                SDL_FColor color);


SDL_Renderer* GetRenderer();
}

#endif  // GPR924_ENGINE_RENDERER_H
