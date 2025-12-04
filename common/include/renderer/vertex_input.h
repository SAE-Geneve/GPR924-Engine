//
// Created by unite on 04.12.2025.
//

#ifndef GPR924_ENGINE_VERTEX_INPUT_H
#define GPR924_ENGINE_VERTEX_INPUT_H
#include "resource/resource.h"
#include "third_party/gl_include.h"

namespace common {

struct VertexInputInfo {
  GLuint vao = 0;
};

class VertexInputDestructor {
  public:
  explicit VertexInputDestructor(VertexInputInfo& vertex_input_info):vertex_input_info_(vertex_input_info) {

  }
  void operator()() const {
    glDeleteVertexArrays(1, &vertex_input_info_.vao);
    vertex_input_info_.vao = 0;
  }
private:
  VertexInputInfo& vertex_input_info_;
};
class VertexInput
    : public core::Resource<VertexInputInfo, VertexInputDestructor> {
 public:
  void Load();
  void Bind();
};

}  // namespace common
#endif  // GPR924_ENGINE_VERTEX_INPUT_H
