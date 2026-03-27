//
// Created by hugze on 13.11.2025.
//

#ifndef SAMPLE_H
#define SAMPLE_H

#include "engine/gui.h"
#include "engine/renderer.h"
#include "engine/system.h"

class Sample : public common::SystemInterface, public common::OnGuiInterface, public common::DrawInterface {
 public:
  void Draw() override = 0;
  void OnGui() override = 0;
  void Begin() override = 0;
  void End() override = 0;
  void Update(float dt) override = 0;
  void FixedUpdate() override = 0;
};

#endif //SAMPLE_H
