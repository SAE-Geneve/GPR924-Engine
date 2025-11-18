/*
Copyright 2025 SAE Institute Switzerland SA

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Contributors: Elias Farhan
Contributors: Anthony Barman
*/

#include "engine/renderer.h"

#include <cmath>
#include <format>

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
    throw std::runtime_error(
        std::format("SDL renderer failed to initialise: {}", SDL_GetError()));
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
                const SDL_FColor color, const int nbSegments) {
  for (int i = 0; i < nbSegments; ++i) {
    const float angle1 =
        2 * core::PI * static_cast<float>(i) / static_cast<float>(nbSegments);
    const float angle2 = 2 * core::PI * static_cast<float>(i + 1) /
                         static_cast<float>(nbSegments);

    const float x1 =
        centerX + radius * static_cast<float>(cos(static_cast<double>(angle1)));
    const float y1 =
        centerY + radius * static_cast<float>(sin(static_cast<double>(angle1)));
    const float x2 =
        centerX + radius * static_cast<float>(cos(static_cast<double>(angle2)));
    const float y2 =
        centerY + radius * static_cast<float>(sin(static_cast<double>(angle2)));

    SDL_Vertex vertices[3];
    vertices[0].position = {centerX, centerY};
    vertices[0].color = color;
    vertices[1].position = {x1, y1};
    vertices[1].color = color;
    vertices[2].position = {x2, y2};
    vertices[2].color = color;

    SDL_RenderGeometry(common::GetRenderer(), nullptr, vertices, 3, nullptr, 0);
  }
}

void DrawAABB(const core::Vec2F pos, const core::Vec2F rectSize, const SDL_FColor color, const bool filled) {
  const SDL_FRect rect(pos.x - rectSize.x / 2,
                 pos.y - rectSize.y / 2, rectSize.x,
                 rectSize.y);

  SDL_SetRenderDrawColorFloat(GetRenderer(), color.r, color.g,
                              color.b, color.a);
  if(filled) SDL_RenderFillRect(GetRenderer(), &rect);
  else SDL_RenderRect(GetRenderer(), &rect);
}

SDL_Renderer* GetRenderer() { return renderer; }
}  // namespace common