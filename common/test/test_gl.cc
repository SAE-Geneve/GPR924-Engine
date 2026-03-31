/*
Copyright 2026 SAE Institute Switzerland SA

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
*/


#include "engine/engine.h"
#include "engine/system.h"
#include "engine/window.h"
#include "gtest/gtest.h"

namespace test_gl {
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
  void FixedUpdate() override {
    ++count_;
    if (count_ > kCloseCount) {
      common::CloseWindow();
    }
  }

protected:
  static constexpr int kCloseCount = 100;
  int count_ = 0;
};
}

TEST(GL, OpenGLESWindow) {
  test_gl::ClosingSystem closingSystem;
  auto windowConfig = common::GetWindowConfig();
  windowConfig.renderer = common::WindowConfig::RendererType::OPENGLES;
  common::SetWindowConfig(windowConfig);
  common::SystemObserverSubject::AddObserver(&closingSystem);
  common::RunEngine();
  common::SystemObserverSubject::RemoveObserver(&closingSystem);
}

TEST(GL, OpenGLWindow) {
  test_gl::ClosingSystem closingSystem;
  auto windowConfig = common::GetWindowConfig();
  windowConfig.renderer = common::WindowConfig::RendererType::OPENGL;
  common::SetWindowConfig(windowConfig);
  common::SystemObserverSubject::AddObserver(&closingSystem);
  common::RunEngine();
  common::SystemObserverSubject::RemoveObserver(&closingSystem);
}