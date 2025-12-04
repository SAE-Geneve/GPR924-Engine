//
// Created by unite on 04.12.2025.
//
#include "renderer/pipeline.h"

#include <iostream>
#include <ostream>
#include <string>

namespace common {

namespace {
GLuint binded_pipeline_name = 0;
}
void Pipeline::Load(const Shader& vertex_shader,
                    const Shader& fragment_shader) {
  auto& pipelineInfo = get();
  pipelineInfo.pipeline_name = glCreateProgram();
  glAttachShader(pipelineInfo.pipeline_name, (*vertex_shader).shader_name);
  glAttachShader(pipelineInfo.pipeline_name, (*fragment_shader).shader_name);
  glLinkProgram(pipelineInfo.pipeline_name);
  GLint program_linked;
  glGetProgramiv(pipelineInfo.pipeline_name, GL_LINK_STATUS, &program_linked);
  if (program_linked != GL_TRUE)
  {
    GLsizei log_length = 0;
    std::string message( 1024, 0);
    glGetProgramInfoLog(pipelineInfo.pipeline_name,
      static_cast<GLsizei>(message.size()),
      &log_length,
      message.data());
    std::cerr << message << std::endl;
    throw std::runtime_error("Error trying to link pipeline");
  }
}
void Pipeline::Bind() {
  if (binded_pipeline_name != get().pipeline_name) {
    glUseProgram(get().pipeline_name);
    binded_pipeline_name = get().pipeline_name;
  }
}
}  // namespace common