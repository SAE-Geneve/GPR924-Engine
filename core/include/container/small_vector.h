//
// Created by efarhan on 3/31/26.
//

#ifndef GPR924_ENGINE_SMALL_VECTOR_H
#define GPR924_ENGINE_SMALL_VECTOR_H

#include <algorithm>
#include <cstdlib>
#include <stdexcept>

namespace core {

template <typename T, std::size_t kCapacity>
class SmallVector {
 public:
  constexpr SmallVector() = default;

  constexpr SmallVector(std::initializer_list<T> list) {
    const auto size = list.size();
    if (size > kCapacity) {
      throw std::runtime_error("SmallVector size is too large");
    }
    std::copy(list.begin(), list.end(), underlying_container_.begin());
    size_ = size;
  }

  constexpr auto begin() { return underlying_container_.begin(); }

  constexpr auto end() { return underlying_container_.begin() + size_; }

  constexpr auto begin() const { return underlying_container_.begin(); }

  constexpr auto end() const { return underlying_container_.begin() + size_; }

  constexpr auto cbegin() const { return underlying_container_.cbegin(); }

  constexpr auto cend() const { return underlying_container_.cbegin() + size_; }

  constexpr void push_back(const T& value) {
    if (size_ == kCapacity) {
      // Over-capacity leads to a crash
      throw std::runtime_error("Over-capacity");
    }
    underlying_container_[size_] = value;
    size_++;
  }

  constexpr void push_back(T&& value) {
    if (size_ == kCapacity) {
      // Over-capacity leads to a crash
      throw std::runtime_error("Over-capacity");
    }
    underlying_container_[size_] = std::move(value);
    size_++;
  }

  constexpr void clear() {
    for (std::size_t i = 0; i < size_; i++) {
      underlying_container_[i] = T{};
    }
    size_ = 0;
  }

  constexpr void resize(std::size_t newSize) {
    if (newSize > kCapacity) {
      throw std::runtime_error("Over-capacity");
    }
    if (size_ == newSize) {
      return;
    }
    if (size_ < newSize) {
      for (auto i = size_; i < newSize; i++) {
        underlying_container_[i] = T{};
      }
    } else {
      for (auto i = newSize; i < size_; i++) {
        underlying_container_[i] = T{};
      }
    }
    size_ = newSize;
  }

  constexpr T& operator[](std::size_t pos) {
    return underlying_container_[pos];
  }

  constexpr const T& operator[](std::size_t pos) const {
    return underlying_container_[pos];
  }

  constexpr typename std::array<T, kCapacity>::const_iterator insert(
      typename std::array<T, kCapacity>::const_iterator pos, const T& value) {
    if (size_ == kCapacity) {
      // Over-capacity leads to a crash
      throw std::runtime_error("Over-capacity");
    }
    const auto index =
        static_cast<std::size_t>(std::distance(cbegin(), pos));
    for (auto i = size_; i > index; i--) {
      underlying_container_[i] = std::move(underlying_container_[i - 1]);
    }
    underlying_container_[index] = value;
    size_++;
    return pos;
  }

  constexpr typename std::array<T, kCapacity>::const_iterator insert(
      typename std::array<T, kCapacity>::const_iterator pos, T&& value) {
    if (size_ == kCapacity) {
      // Over-capacity leads to a crash
      throw std::runtime_error("Over-capacity");
    }
    const auto index =
        static_cast<std::size_t>(std::distance(cbegin(), pos));
    for (auto i = size_; i > index; i--) {
      underlying_container_[i] = std::move(underlying_container_[i - 1]);
    }
    underlying_container_[index] = std::move(value);
    size_++;
    return pos;
  }

  constexpr auto erase(typename std::array<T, kCapacity>::iterator pos) {
    std::move(pos + 1, end(), pos);
    size_--;
    return pos;
  }

  constexpr auto erase(typename std::array<T, kCapacity>::const_iterator pos) {
    const auto index = static_cast<std::size_t>(std::distance(cbegin(), pos));
    for (auto i = index; i < size_ - 1; i++) {
      underlying_container_[i] = std::move(underlying_container_[i + 1]);
    }
    size_--;
    return pos;
  }

  constexpr auto capacity() const { return kCapacity; }

  constexpr auto size() const { return size_; }

  constexpr T& front() { return underlying_container_.front(); }
  constexpr const T& front() const { return underlying_container_.front(); }
  constexpr T& back() { return underlying_container_[size_ - 1]; }
  constexpr const T& back() const { return underlying_container_[size_ - 1]; }

  constexpr void pop_back() {
    if (size_ == 0) {
      throw std::runtime_error("pop_back on empty SmallVector");
    }
    underlying_container_[size_ - 1] = T{};
    --size_;
  }

  constexpr auto data() noexcept { return underlying_container_.data(); }
  [[nodiscard]] constexpr bool is_full() const { return size_ == kCapacity; }
  [[nodiscard]] constexpr bool is_empty() const { return size_ == 0; }

  constexpr bool operator==(const SmallVector& other) const {
    return size_ == other.size_ &&
           std::equal(begin(), end(), other.begin());
  }
  constexpr bool operator!=(const SmallVector& other) const {
    return !operator==(other);
  }

 private:
  std::array<T, kCapacity> underlying_container_{};
  std::size_t size_ = 0;
};
}  // namespace core

#endif  // GPR924_ENGINE_SMALL_VECTOR_H
