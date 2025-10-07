#include <imgui.h>

#include "engine/engine.h"
#include "engine/gui.h"
#include "engine/window.h"
#include "gtest/gtest.h"

class ClosingSystem : public common::SystemInterface {
  public:
  void Begin() override{}
  void End() override{}
  void Update([[maybe_unused]]float dt) override {
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

class ClosingSystemWithGui : public ClosingSystem, public common::OnGuiInterface {
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