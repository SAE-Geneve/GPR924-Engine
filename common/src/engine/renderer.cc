#include "engine/renderer.h"

#include <format>

#include "engine/gui.h"
#include "engine/window.h"

namespace common {

namespace {
SDL_Renderer* renderer = nullptr;
}
void BeginRenderer() {
  auto* window = GetWindow();
  renderer = SDL_CreateRenderer(window, nullptr);
  if (renderer == nullptr) {
    throw std::runtime_error(std::format("SDL renderer failed to initialise: {}", SDL_GetError()));
  }

  BeginGuiRenderer();
}
void UpdateRenderer() { UpdateGuiRenderer(); }
void DrawRenderer() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  for (auto& drawInterface : DrawObserverSubject::GetObservers()) {
    drawInterface->PreDraw();
  }

  for (auto& drawInterface : DrawObserverSubject::GetObservers()) {
    drawInterface->Draw();
  }

  for (auto& drawInterface : DrawObserverSubject::GetObservers()) {
    drawInterface->PostDraw();
  }
  DrawGuiRenderer();

  SDL_RenderPresent(renderer);
}
void EndRenderer() {
  EndGuiRenderer();
  SDL_DestroyRenderer(renderer);
}
SDL_Renderer* GetRenderer() { return renderer; }
}  // namespace common