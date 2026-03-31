//
// Created by efarhan on 3/31/26.
//

#ifndef GPR924_ENGINE_CHECKSUM_H
#define GPR924_ENGINE_CHECKSUM_H

#include <cstdint>
#include <initializer_list>
#include <stdexcept>

#include "container/small_vector.h"
#include "network/adler32.h"

namespace common {

template<int kArgCount>
struct Checksum
{
  Checksum(std::initializer_list<uint32_t> list )
  {
    if (list.size() > kArgCount) {
      throw std::runtime_error("Over-capacity");
    }
    for(std::size_t i = 0; i < list.size(); i++)
    {
      data_.push_back(data(list)[i]);
    }
  }
  bool operator==(const Checksum& other) const
  {
    return data_ == other.data_;
  }
  bool operator!=(const Checksum& other) const
  {
    return data_ != other.data_;
  }
  std::uint32_t operator[](std::size_t index) const
  {
    return data_[index];
  }

  [[nodiscard]] explicit operator std::uint32_t() const
  {
    Adler32 adler32{};
    adler32.Add(data_);
    return adler32.value();
  }

  template<int otherArgCount>
  void push_back(const Checksum<otherArgCount>& otherChecksum )
  {
    for(int i = 0; i < otherArgCount; i++)
    {
      data_.push_back(otherChecksum[i]);
    }
  }
private:
  core::SmallVector<std::uint32_t, kArgCount> data_{};
};
}
#endif //GPR924_ENGINE_CHECKSUM_H
