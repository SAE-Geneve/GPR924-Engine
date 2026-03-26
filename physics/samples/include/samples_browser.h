//
// Created by hugze on 13.11.2025.
//

#ifndef SAMPLES_BROWSER_H
#define SAMPLES_BROWSER_H

#include <memory>
#include "maths/vec2.h"
#include "sample.h"

namespace browser {
class SamplesBrowser : public Sample {

public:

  void CreateSamples();
  explicit SamplesBrowser(core::Vec2F screen_size_) : screen_size(screen_size_){CreateSamples();}
  void OnGui() override;
  void Draw() override;
  void Begin() override;
  void End() override;
  void Update(float dt) override;
  void FixedUpdate() override;

 private:
  core::Vec2F screen_size;
  std::vector<std::pair<std::string, std::unique_ptr<Sample>>> samples;
  int current_sample_index = 0;
};

}

#endif //SAMPLES_BROWSER_H
