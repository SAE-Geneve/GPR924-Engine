#include <imgui.h>

#include "engine/engine.h"
#include "engine/gui.h"
#include "engine/renderer.h"
#include "engine/window.h"
#include "gtest/gtest.h"

#include "third_party/sdl3_include.h"

class ClosingSystem : public common::SystemInterface {
 public:
  void Begin() override {}
  void End() override {}
  void Update([[maybe_unused]] float dt) override {
    ++count_;
    if (count_ > kCloseCount) {
      common::CloseWindow();
    }
  }

 protected:
  static constexpr int kCloseCount = 100;
  int count_ = 0;
};

TEST(Engine, RunEngine) {
  ClosingSystem closingSystem;
  common::SystemObserverSubject::AddObserver(&closingSystem);
  common::RunEngine();
  common::SystemObserverSubject::RemoveObserver(&closingSystem);
}

class ClosingSystemWithGui : public ClosingSystem,
                             public common::OnGuiInterface {
 public:
  void OnGui() override {
    ImGui::Begin("New Gui Window");
    ImGui::Text("Count: %d", count_);
    ImGui::End();
  }
};

TEST(Engine, Gui) {
  ClosingSystemWithGui closingSystem;
  common::SystemObserverSubject::AddObserver(&closingSystem);
  common::OnGuiObserverSubject::AddObserver(&closingSystem);
  common::RunEngine();

  common::OnGuiObserverSubject::RemoveObserver(&closingSystem);
  common::SystemObserverSubject::RemoveObserver(&closingSystem);
}

class DrawSomething : public common::DrawInterface {
public:
  void Draw() override {
    auto* renderer = common::GetRenderer();

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_FRect rect{.x = 100, .y = 100, .w = 200, .h = 200};
    SDL_RenderFillRect(renderer, &rect);
  }
};


TEST(Engine, Render) {
  DrawSomething draw_something;
  common::DrawObserverSubject::AddObserver(&draw_something);
  common::RunEngine();

  common::DrawObserverSubject::RemoveObserver(&draw_something);
}