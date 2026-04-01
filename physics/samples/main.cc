//
// Created by hugze on 13.11.2025.
//

#include "engine/engine.h"
#include "engine/window.h"
#include "samples_browser.h"
#include "world.h"

static int constexpr WindowWidth = 1280;
static int constexpr WindowHeight = 720;

int main() {
  common::WindowConfig windowConfig{};
  windowConfig.title = "Sample Browser";
  windowConfig.width = WindowWidth;
  windowConfig.height = WindowHeight;
  common::SetWindowConfig(windowConfig);

  physics::set_world_bounds(physics::AABB({0,0}, {WindowWidth, WindowHeight}));

  browser::SamplesBrowser Browser{{WindowWidth, WindowHeight}};
  common::SystemObserverSubject::AddObserver(&Browser);
  common::OnGuiObserverSubject::AddObserver(&Browser);

  common::RunEngine();

  return 0;
}