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

#ifndef GPR924_ENGINE_VERTEX_INPUT_H
#define GPR924_ENGINE_VERTEX_INPUT_H

#include <ranges>

#include "resource/resource.h"
#include "third_party/gl_include.h"

namespace common {

struct Buffer {
  GLuint name;
};

class BufferDestructor {
 public:
  explicit BufferDestructor(Buffer& buffer) : buffer_(buffer) {}
  void operator()() const {
    glDeleteBuffers(1, &buffer_.name);
    buffer_.name = 0;
  }

 private:
  Buffer& buffer_;
};

class VertexBuffer : public core::Resource<Buffer, BufferDestructor> {
public:
  void Load();
  template <std::ranges::contiguous_range Range>
  void UploadRange(const Range& range, GLenum usage = GL_STATIC_DRAW) {

    Bind();
    const auto totalSize = std::ranges::size(range)*sizeof(std::ranges::range_value_t<Range>);
    glBufferData(GL_VERTEX_ARRAY, totalSize, range.data(), usage);
  }
  void Bind() const;
};

struct VertexBufferAttribute {
  GLuint location;
  GLint size;
  GLenum type;
  GLsizei stride;
  size_t offset;
};

class IndexBuffer : public core::Resource<Buffer, BufferDestructor> {
public:
  void Load();
  template <std::ranges::contiguous_range Range>
  void UploadRange(const Range& range, GLenum usage = GL_STATIC_DRAW) {

    Bind();
    using value_type = std::ranges::range_value_t<Range>;
    static_assert(std::is_same_v<value_type, int32_t> || std::is_same_v<value_type, int16_t>, "Element buffer needs either int32 or int16");
    const auto totalSize = std::ranges::size(range)*sizeof(std::ranges::range_value_t<Range>);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, totalSize, range.data(), usage);
  }
  void Bind() const;
};

struct VertexInputInfo {
  GLuint vao = 0;
};

class VertexInputDestructor {
 public:
  explicit VertexInputDestructor(VertexInputInfo& vertex_input_info)
      : vertex_input_info_(vertex_input_info) {}
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
  void BindVertexBuffer(const VertexBuffer& vbo, std::span<const VertexBufferAttribute> attributes);
  void BindIndexBuffer(const IndexBuffer& ebo);
  void Bind();
};

}  // namespace common
#endif  // GPR924_ENGINE_VERTEX_INPUT_H
