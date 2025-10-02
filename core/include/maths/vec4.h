#ifndef VEC4_H
#define VEC4_H

#include <cmath>
#include <concepts>

namespace core::maths
{
  template<typename Vector4T, typename T>
  concept IsVector4 = requires(Vector4T v)
  {
    { v.x } -> std::same_as<T&>;
    { v.y } -> std::same_as<T&>;
  };
}

#endif //VEC4_H
