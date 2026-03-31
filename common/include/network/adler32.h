#ifndef GPR924_ENGINE_ADLER32_INCLUDE_H
#define GPR924_ENGINE_ADLER32_INCLUDE_H

/*
Copyright 2026 SAE Institute Switzerland SA

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

#include "maths/vec2.h"

#include <bit>
#include <cstdint>
#include <ranges>
#include <span>


namespace common {

/**
 * @brief Incremental Adler-32 checksum calculator.
 *
 * Used by the rollback engine to compute deterministic checksums of game state.
 * Supports adding various types (scalars, vectors, timers, ranges) to the
 * running checksum. Call Reset() to start a new checksum, then Add() values
 * in a deterministic order, and retrieve the result with value().
 *
 * @par Example
 * @code
 * Adler32 adler;
 * adler.Add(position.x);
 * adler.Add(position.y);
 * adler.Add(health);
 * uint32_t checksum = adler.value();
 * @endcode
 *
 * @warning The order in which values are added matters. All clients must add
 *          values in the same order to produce matching checksums.
 */
class Adler32 {
  uint32_t a = 1, b = 0;
  static constexpr uint32_t kModAdler = 65521;

 public:
  /** @brief Resets the checksum to its initial state. */
  void Reset() noexcept {
    a = 1;
    b = 0;
  }

  /**
   * @brief Returns the current Adler-32 checksum value.
   * @return The 32-bit checksum (upper 16 bits = b, lower 16 bits = a).
   */
  [[nodiscard]] uint32_t value() const noexcept { return (b << 16) | a; }

  /**
   * @brief Adds a 2D vector to the checksum (x then y).
   * @tparam T Component type.
   * @param v The vector to hash.
   * @return Updated checksum value.
   */
  template <class T>
  uint32_t Add(core::Vec2<T> v) {
    Add(v.x);
    return Add(v.y);
  }

  /**
   * @brief Adds every element of a range to the checksum.
   * @tparam R A type satisfying std::ranges::range.
   * @param range The range whose elements to hash.
   * @return Updated checksum value after the last element.
   */
  template <std::ranges::range R>
  uint32_t Add(R&& range) {
    uint32_t result = 0;
    for (auto&& elem : range) {
      result = Add(elem);
    }
    return result;
  }

  /**
   * @brief Adds an arbitrary trivial type to the checksum via bit_cast.
   *
   * Supports 1, 2, 4, and 8-byte types. The value is reinterpreted as
   * unsigned integers and folded into the checksum.
   *
   * @tparam T The type to hash (must not be a range).
   * @param data The value to hash.
   * @return Updated checksum value.
   */
  template <typename T>
    requires(!std::ranges::range<T>)
  uint32_t Add(const T& data) {
    if constexpr (sizeof(T) == sizeof(uint32_t)) {
      a = (a + std::bit_cast<uint32_t>(data)) % kModAdler;
      b = (b + a) % kModAdler;
      return (b << 16) | a;
    } else if constexpr (sizeof(T) == sizeof(uint16_t)) {
      a = (a + std::bit_cast<uint16_t>(data)) % kModAdler;
      b = (b + a) % kModAdler;
      return (b << 16) | a;
    } else if constexpr (sizeof(T) == sizeof(uint8_t)) {
      a = (a + std::bit_cast<uint8_t>(data)) % kModAdler;
      b = (b + a) % kModAdler;
      return (b << 16) | a;
    } else if constexpr (sizeof(T) == sizeof(uint64_t)) {
      const auto tmp_value = std::bit_cast<uint64_t>(data);
      const auto lower_value = static_cast<uint32_t>(tmp_value);
      const auto upper_value = static_cast<uint32_t>(tmp_value >> 32);
      Add(lower_value);
      return Add(upper_value);
    } else {
      throw std::runtime_error("Unsupported type");
    }
  }

  /**
   * @brief Adds raw bytes to the checksum.
   * @param data Pointer to the byte array.
   * @param len  Number of bytes to process.
   * @return Updated checksum value.
   */
  uint32_t Add(std::span<char> raw_data) {
    // Process each byte of the data in order
    for (auto & data : raw_data) {
      const auto v = static_cast<uint32_t>(data);
      a = (a + v) % kModAdler;
      b = (b + a) % kModAdler;
    }

    return (b << 16) | a;
  }
};

}  // namespace splash

#endif //GPR924_ENGINE_ADLER32_INCLUDE_H
