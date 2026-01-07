#ifndef GPR924_ENGINE_TEXTURE_H
#define GPR924_ENGINE_TEXTURE_H

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

Contributors: Elias Farhan, Jemoel Ablay
*/

#include <span>
#include <string_view>
#include "resource/resource.h"
#include "third_party/gl_include.h"

namespace common {


struct TextureInfo {
  GLuint texture_name;
  GLenum texture_target;
};

class TextureDestructor {
public:
  explicit TextureDestructor(TextureInfo& texture_info)
      : texture_info_(texture_info) {}
  void operator()() const {
    glDeleteTextures(1, &texture_info_.texture_name);
    texture_info_.texture_name = 0;
  }
private:
  TextureInfo& texture_info_;
};

enum class WrappingMode : uint8_t {
  REPEAT,
  CLAMP_TO_EDGE,
};
enum class FilteringMode : uint8_t {
  NEAREST,
  LINEAR,
};
struct TextureLoadInfo {
  WrappingMode wrapping_mode = WrappingMode::REPEAT;
  FilteringMode filtering_mode = FilteringMode::LINEAR;
  uint8_t required_channels = 0;
  bool generate_mipmaps = true;
};

class Texture : public core::Resource<TextureInfo, TextureDestructor> {
public:
  void Load(std::string_view path, TextureLoadInfo texture_load_info = {});
  void Load(void* data, int width, int height, int channels, TextureLoadInfo texture_load_info = {});
  void LoadCubeMap(std::span<const std::string_view> faces);
  void Bind() const;
};

}
#endif  // GPR924_ENGINE_TEXTURE_H
