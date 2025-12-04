
/*
Copyright 2025 SAE Institute Switzerland SA

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
  glAttachShader(pipelineInfo.pipeline_name, vertex_shader->shader_name);
  glAttachShader(pipelineInfo.pipeline_name, fragment_shader->shader_name);
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

void Pipeline::SetInt(const char* name, const int value)
{
  const GLint loc = glGetUniformLocation(get().pipeline_name, name);
  glUniform1i(loc, value);
}

void Pipeline::SetFloat(const char* name, const float value)
{
  const GLint loc = glGetUniformLocation(get().pipeline_name, name);
  glUniform1f(loc, value);
}

void Pipeline::SetMat4(const char* name, const float* mat)
{
  const GLint loc = glGetUniformLocation(get().pipeline_name, name);
  glUniformMatrix4fv(loc, 1, GL_FALSE, mat);
}
}  // namespace common