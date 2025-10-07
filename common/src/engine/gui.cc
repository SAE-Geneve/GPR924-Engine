#include "engine/gui.h"

#include "engine/renderer.h"
#include "engine/window.h"
#include "third_party/imgui_include.h"

namespace common {
namespace {
SDL_Renderer* renderer_;
}
void BeginGuiRenderer() {
  renderer_ = GetRenderer();
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  ImGuiStyle& style = ImGui::GetStyle();
  float mainScale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
  style.ScaleAllSizes(
      mainScale);  // Bake a fixed style scale. (until we have a solution for
                   // dynamic style scaling, changing this requires resetting
                   // Style + calling this again)

  ImGui_ImplSDL3_InitForSDLRenderer(GetWindow(), renderer_);
  ImGui_ImplSDLRenderer3_Init(renderer_);
}
void UpdateGuiRenderer() {
  ImGui_ImplSDLRenderer3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();

  for (auto* guiInterface : OnGuiObserverSubject::GetObservers()) {
    if (guiInterface == nullptr) continue;
    guiInterface->OnGui();
  }
}
void EndGuiRenderer() {
  ImGui_ImplSDLRenderer3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();
}
void DrawGuiRenderer() {
  ImGui::Render();
  ImGuiIO& io = ImGui::GetIO();
  SDL_SetRenderScale(renderer_, io.DisplayFramebufferScale.x,
                     io.DisplayFramebufferScale.y);
  ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer_);
}
void ManageGuiEvent(const SDL_Event& event) {
  ImGui_ImplSDL3_ProcessEvent(&event);
}
}  // namespace common