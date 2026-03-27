//
// Created by hugze on 13.11.2025.
//

#include <imgui.h>

#include "samples_browser.h"

#include "bounce_sample.h"
#include "colliders_sample.h"
#include "solar_sample.h"

namespace browser {
void SamplesBrowser::CreateSamples() {
  solar::SolarSample physicSolarSystem(screen_size);
  samples.emplace_back("Solar",
                       std::make_unique<solar::SolarSample>(physicSolarSystem));

  colliders::CollidersSample colliderSample(screen_size);
  samples.emplace_back(
      "Colliders",
      std::make_unique<colliders::CollidersSample>(colliderSample));

  colliders::BounceSample bounceSample(screen_size);
  samples.emplace_back("Bounce",
                       std::make_unique<colliders::BounceSample>(bounceSample));
}

void SamplesBrowser::OnGui() {
  ImGui::Begin("Sample Browser");

  static ImGuiComboFlags flags = ImGuiComboFlags_WidthFitPreview;
  const char *items[] = {"Solar", "Colliders", "Bounce"};
  const char *combo_preview_value = items[current_sample_index];

  if (ImGui::BeginCombo("Samples", combo_preview_value, flags)) {
    for (int n = 0; n < IM_ARRAYSIZE(items); n++) {
      bool is_selected = (current_sample_index == n);
      if (ImGui::Selectable(items[n], is_selected)) {
        if (current_sample_index >= 0) {
          samples[current_sample_index].second->End();
        }
        current_sample_index = n;
        samples[current_sample_index].second->Begin();
      }
      if (is_selected) {
        ImGui::SetItemDefaultFocus();
      }
    }
    ImGui::EndCombo();
  }

  ImGui::End();
  if (current_sample_index >= 0) {
    samples[current_sample_index].second->OnGui();
  }
}

void SamplesBrowser::Draw() {
  if (current_sample_index >= 0) {
    samples[current_sample_index].second->Draw();
  }
}

void SamplesBrowser::Begin() {
  if (current_sample_index >= 0) {
    samples[current_sample_index].second->Begin();
  }
}

void SamplesBrowser::End() {
  if (current_sample_index >= 0) {
    samples[current_sample_index].second->End();
  }
}

void SamplesBrowser::Update(float dt) {
  // if (current_sample_index >= 0) {
  //   samples[current_sample_index].second->Update(dt);
  // }
}

void SamplesBrowser::FixedUpdate() {
  if (current_sample_index >= 0) {
    samples[current_sample_index].second->FixedUpdate();
  }
}
}  // namespace browser
