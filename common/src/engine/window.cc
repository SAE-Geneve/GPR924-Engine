#include "engine/window.h"

#include <format>

#include "engine/gui.h"

namespace common {
namespace {
WindowConfig windowConfig;
SDL_Window* window = nullptr;
bool isOpen = false;
}  // namespace
SDL_Window* GetWindow() { return window; }
void BeginWindow() {
  SDL_WindowFlags flags = 0;
  if (windowConfig.resizable) {
    flags |= SDL_WINDOW_RESIZABLE;
  }
  window = SDL_CreateWindow(windowConfig.title.data(), windowConfig.width,
                            windowConfig.height, flags);
  if (window == nullptr) {
    throw std::runtime_error(
        std::format("Failed to create window, error: {}", SDL_GetError()));
  }
  isOpen = true;
}
void UpdateWindow() {
  SDL_Event e;
  // Handle events on queue
  while (SDL_PollEvent(&e) != 0) {
    // User requests quit
    if (e.type == SDL_EVENT_QUIT ||
        e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
      isOpen = false;
    }
    ManageGuiEvent(e);
    for (auto* eventInterface : OnEventObserverSubject::GetObservers()) {
      eventInterface->OnEvent(e);
    }
  }
}
void EndWindow() { SDL_DestroyWindow(window); }
std::pair<int, int> GetWindowSize() {
  std::pair<int, int> windowSize;
  SDL_GetWindowSize(window, &windowSize.first, &windowSize.second);
  return windowSize;
}
bool IsWindowOpen() { return isOpen; }
void SetWindowConfig(const WindowConfig& config) { windowConfig = config; }
void CloseWindow() { isOpen = false; }
}  // namespace common