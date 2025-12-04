#ifndef GPR924_ENGINE_PIPELINE_H
#define GPR924_ENGINE_PIPELINE_H


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

  void SetInt(const char* name, int value);
  void SetFloat(const char* name, float value);
};
}
#endif  // GPR924_ENGINE_PIPELINE_H
