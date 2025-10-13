#include "engine/renderer.h"

#include <format>
#include <cmath>

#include "engine/gui.h"
#include "engine/window.h"
#include "maths/constant.h"

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

void DrawCircle(const float centerX, const float centerY, const float radius,
                const SDL_FColor color)
{
  constexpr int nbSegments = 72;
  for (int i = 0; i < nbSegments; ++i)
  {
    const float angle1 = 2 * core::PI * static_cast<float>(i) / nbSegments;
    const float angle2 = 2 * core::PI * static_cast<float>(i + 1) / nbSegments;

    const float x1 = centerX + radius * static_cast<float>(cos(static_cast<float>(angle1)));
    const float y1 = centerY + radius * static_cast<float>(sin(static_cast<float>(angle1)));
    const float x2 = centerX + radius * static_cast<float>(cos(static_cast<float>(angle2)));
    const float y2 = centerY + radius * static_cast<float>(sin(static_cast<float>(angle2)));

    SDL_Vertex vertices[3];
    vertices[0].position = { centerX, centerY };
    vertices[0].color = color;
    vertices[1].position = { x1, y1 };
    vertices[1].color = color;
    vertices[2].position = { x2, y2 };
    vertices[2].color = color;

    SDL_RenderGeometry(common::GetRenderer(), nullptr, vertices, 3, nullptr, 0);
  }
}

SDL_Renderer* GetRenderer() { return renderer; }
}  // namespace common