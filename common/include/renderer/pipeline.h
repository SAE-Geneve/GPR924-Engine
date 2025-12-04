//
// Created by unite on 04.12.2025.
//

#ifndef GPR924_ENGINE_PIPELINE_H
#define GPR924_ENGINE_PIPELINE_H

#include <string_view>

#include "resource/resource.h"
#include "shader.h"
#include "third_party/gl_include.h"

namespace common {

struct PipelineInfo {
  GLuint pipeline_name = 0;
};

class PipelineDestructor {
public:
  explicit PipelineDestructor(PipelineInfo& pipeline_info): pipeline_info_(pipeline_info) {}
  void operator()() const {
    glDeleteProgram(pipeline_info_.pipeline_name);
    pipeline_info_.pipeline_name = 0;
  }
private:
  PipelineInfo& pipeline_info_;
};

class Pipeline : core::Resource<PipelineInfo, PipelineDestructor> {
public:
  void Load(const Shader& vertex_shader, const Shader& fragment_shader);
  void Bind();
};
}
#endif  // GPR924_ENGINE_PIPELINE_H
