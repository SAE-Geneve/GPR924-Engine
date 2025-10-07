#ifndef GPR924_ENGINE_WINDOW_H
#define GPR924_ENGINE_WINDOW_H

#include "third_party/sdl3_include.h"
#include "utils/observer.h"

namespace common {
class OnEventInterface
{
public:
  virtual ~OnEventInterface() = default;
  virtual void OnEvent(const SDL_Event& event) = 0;
};

struct WindowConfig {
  std::string title = "GPR924-Engine";
  int width = 1280;
  int height = 720;
  bool fullscreen = false;
  bool resizable = false;
};

using OnEventObserverSubject = ObserverSubject<OnEventInterface>;

SDL_Window* GetWindow();

void BeginWindow();
void UpdateWindow();
void EndWindow();
std::pair<int, int> GetWindowSize();
bool IsWindowOpen();
void SetWindowConfig(const WindowConfig& config);
void CloseWindow();
}
#endif  // GPR924_ENGINE_WINDOW_H
