#include "engine/engine.h"

#include <format>

#include "engine/renderer.h"
#include "engine/window.h"
#include "third_party/sdl3_include.h"

namespace common {
namespace {
float dt = 0.0f;
Uint64 currentTime = 0u;
}  // namespace
static void BeginEngine() {
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
    throw(std::format("SDL failed to initialise: {}", SDL_GetError()));
  }
  BeginWindow();
  BeginRenderer();
  for (auto* system : SystemObserverSubject::GetObservers()) {
    if (system == nullptr) continue;
    system->Begin();
  }
}
static void EndEngine() {
  for (auto* system : SystemObserverSubject::GetObservers()) {
    if (system == nullptr) continue;
    system->End();
  }
  EndRenderer();
  EndWindow();

  SDL_Quit();
}
void RunEngine() {
  BeginEngine();
  auto freq = static_cast<double>(SDL_GetPerformanceFrequency());
  Uint64 previous = SDL_GetPerformanceCounter();
  while (IsWindowOpen()) {
    currentTime = SDL_GetPerformanceCounter();
    auto delta = static_cast<double>(currentTime - previous);
    previous = currentTime;

    dt = static_cast<float>(delta / freq);
    UpdateWindow();
    UpdateRenderer();
    for (auto& system : SystemObserverSubject::GetObservers()) {
      if (system == nullptr) continue;
      system->Update(dt);
    }
    DrawRenderer();
  }
  EndEngine();
}
float GetDeltaTime() { return dt; }
}  // namespace common