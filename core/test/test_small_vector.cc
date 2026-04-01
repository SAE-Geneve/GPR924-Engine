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

#include <gtest/gtest.h>

#include <string>

#include "container/small_vector.h"

struct MoveOnly {
  int value = 0;
  MoveOnly() = default;
  explicit MoveOnly(int v) : value(v) {}
  MoveOnly(const MoveOnly&) = delete;
  MoveOnly& operator=(const MoveOnly&) = delete;
  MoveOnly(MoveOnly&& other) noexcept : value(other.value) { other.value = 0; }
  MoveOnly& operator=(MoveOnly&& other) noexcept {
    value = other.value;
    other.value = 0;
    return *this;
  }
};

// --- Construction ---

TEST(SmallVector, DefaultConstruction) {
  core::SmallVector<int, 8> v;
  EXPECT_EQ(v.size(), 0);
  EXPECT_TRUE(v.is_empty());
  EXPECT_FALSE(v.is_full());
  EXPECT_EQ(v.capacity(), 8);
}

TEST(SmallVector, InitializerListConstruction) {
  core::SmallVector<int, 8> v = {1, 2, 3};
  EXPECT_EQ(v.size(), 3);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
}

TEST(SmallVector, InitializerListOverCapacity) {
  EXPECT_THROW((core::SmallVector<int, 2>{1, 2, 3}), std::runtime_error);
}

TEST(SmallVector, CopyConstruction) {
  core::SmallVector<int, 8> a = {1, 2, 3};
  auto b = a;
  EXPECT_EQ(b.size(), 3);
  EXPECT_EQ(b[0], 1);
  EXPECT_EQ(b[2], 3);
  b[0] = 99;
  EXPECT_EQ(a[0], 1);
}

TEST(SmallVector, MoveConstruction) {
  core::SmallVector<int, 8> a = {10, 20};
  auto b = std::move(a);
  EXPECT_EQ(b.size(), 2);
  EXPECT_EQ(b[0], 10);
  EXPECT_EQ(b[1], 20);
}

TEST(SmallVector, CopyAssignment) {
  core::SmallVector<int, 8> a = {1, 2};
  core::SmallVector<int, 8> b;
  b = a;
  EXPECT_EQ(b.size(), 2);
  EXPECT_EQ(b[0], 1);
  EXPECT_EQ(b[1], 2);
}

TEST(SmallVector, MoveAssignment) {
  core::SmallVector<int, 8> a = {5, 6};
  core::SmallVector<int, 8> b;
  b = std::move(a);
  EXPECT_EQ(b.size(), 2);
  EXPECT_EQ(b[0], 5);
  EXPECT_EQ(b[1], 6);
}

// --- push_back ---

TEST(SmallVector, PushBackSingle) {
  core::SmallVector<int, 4> v;
  v.push_back(42);
  EXPECT_EQ(v.size(), 1);
  EXPECT_EQ(v[0], 42);
}

TEST(SmallVector, PushBackMultiple) {
  core::SmallVector<int, 8> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  EXPECT_EQ(v.size(), 3);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
}

TEST(SmallVector, PushBackToCapacity) {
  core::SmallVector<int, 3> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  EXPECT_TRUE(v.is_full());
  EXPECT_EQ(v.size(), 3);
}

TEST(SmallVector, PushBackOverCapacity) {
  core::SmallVector<int, 2> v = {1, 2};
  EXPECT_THROW(v.push_back(3), std::runtime_error);
}

TEST(SmallVector, PushBackLvalue) {
  core::SmallVector<int, 4> v;
  const int val = 7;
  v.push_back(val);
  EXPECT_EQ(v[0], 7);
}

TEST(SmallVector, PushBackRvalue) {
  core::SmallVector<std::string, 4> v;
  std::string s = "hello";
  v.push_back(std::move(s));
  EXPECT_EQ(v[0], "hello");
}

// --- pop_back ---

TEST(SmallVector, PopBackSingle) {
  core::SmallVector<int, 4> v = {42};
  v.pop_back();
  EXPECT_EQ(v.size(), 0);
  EXPECT_TRUE(v.is_empty());
}

TEST(SmallVector, PopBackMultiple) {
  core::SmallVector<int, 4> v = {1, 2, 3};
  v.pop_back();
  v.pop_back();
  EXPECT_EQ(v.size(), 1);
  EXPECT_EQ(v[0], 1);
}

TEST(SmallVector, PopBackOnEmpty) {
  core::SmallVector<int, 4> v;
  EXPECT_THROW(v.pop_back(), std::runtime_error);
}

// --- Element access ---

TEST(SmallVector, OperatorBracketRead) {
  core::SmallVector<int, 4> v = {10, 20, 30};
  EXPECT_EQ(v[0], 10);
  EXPECT_EQ(v[1], 20);
  EXPECT_EQ(v[2], 30);
}

TEST(SmallVector, OperatorBracketWrite) {
  core::SmallVector<int, 4> v = {1, 2, 3};
  v[1] = 99;
  EXPECT_EQ(v[1], 99);
}

TEST(SmallVector, ConstOperatorBracket) {
  const core::SmallVector<int, 4> v = {10, 20};
  EXPECT_EQ(v[0], 10);
  EXPECT_EQ(v[1], 20);
}

TEST(SmallVector, Front) {
  core::SmallVector<int, 4> v = {5, 6, 7};
  EXPECT_EQ(v.front(), 5);
  v.front() = 99;
  EXPECT_EQ(v[0], 99);
}

TEST(SmallVector, ConstFront) {
  const core::SmallVector<int, 4> v = {5, 6};
  EXPECT_EQ(v.front(), 5);
}

TEST(SmallVector, Back) {
  core::SmallVector<int, 4> v = {1, 2, 3};
  EXPECT_EQ(v.back(), 3);
  v.back() = 99;
  EXPECT_EQ(v[2], 99);
}

TEST(SmallVector, ConstBack) {
  const core::SmallVector<int, 4> v = {1, 2, 3};
  EXPECT_EQ(v.back(), 3);
}

TEST(SmallVector, Data) {
  core::SmallVector<int, 4> v = {10, 20};
  auto* ptr = v.data();
  EXPECT_EQ(*ptr, 10);
  EXPECT_EQ(*(ptr + 1), 20);
}

// --- Size and capacity ---

TEST(SmallVector, SizeAfterOperations) {
  core::SmallVector<int, 8> v;
  EXPECT_EQ(v.size(), 0);
  v.push_back(1);
  EXPECT_EQ(v.size(), 1);
  v.push_back(2);
  EXPECT_EQ(v.size(), 2);
  v.pop_back();
  EXPECT_EQ(v.size(), 1);
  v.clear();
  EXPECT_EQ(v.size(), 0);
  v.resize(5);
  EXPECT_EQ(v.size(), 5);
}

TEST(SmallVector, IsEmptyAndIsFull) {
  core::SmallVector<int, 2> v;
  EXPECT_TRUE(v.is_empty());
  EXPECT_FALSE(v.is_full());
  v.push_back(1);
  EXPECT_FALSE(v.is_empty());
  EXPECT_FALSE(v.is_full());
  v.push_back(2);
  EXPECT_FALSE(v.is_empty());
  EXPECT_TRUE(v.is_full());
}

// --- clear ---

TEST(SmallVector, ClearResetsSize) {
  core::SmallVector<int, 4> v = {1, 2, 3};
  v.clear();
  EXPECT_EQ(v.size(), 0);
  EXPECT_TRUE(v.is_empty());
}

TEST(SmallVector, PushAfterClear) {
  core::SmallVector<int, 4> v = {1, 2, 3};
  v.clear();
  v.push_back(10);
  EXPECT_EQ(v.size(), 1);
  EXPECT_EQ(v[0], 10);
}

TEST(SmallVector, ClearWithStrings) {
  core::SmallVector<std::string, 4> v;
  v.push_back("hello");
  v.push_back("world");
  v.clear();
  EXPECT_EQ(v.size(), 0);
  v.push_back("after");
  EXPECT_EQ(v[0], "after");
}

// --- resize ---

TEST(SmallVector, ResizeGrow) {
  core::SmallVector<int, 8> v = {1, 2};
  v.resize(5);
  EXPECT_EQ(v.size(), 5);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 0);
  EXPECT_EQ(v[3], 0);
  EXPECT_EQ(v[4], 0);
}

TEST(SmallVector, ResizeShrink) {
  core::SmallVector<int, 8> v = {1, 2, 3, 4, 5};
  v.resize(2);
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
}

TEST(SmallVector, ResizeSameSize) {
  core::SmallVector<int, 4> v = {1, 2};
  v.resize(2);
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
}

TEST(SmallVector, ResizeOverCapacity) {
  core::SmallVector<int, 4> v;
  EXPECT_THROW(v.resize(5), std::runtime_error);
}

TEST(SmallVector, ResizeWithStrings) {
  core::SmallVector<std::string, 8> v;
  v.push_back("a");
  v.push_back("b");
  v.push_back("c");
  v.resize(1);
  EXPECT_EQ(v.size(), 1);
  EXPECT_EQ(v[0], "a");
  v.push_back("d");
  EXPECT_EQ(v[1], "d");
}

// --- insert ---

TEST(SmallVector, InsertAtBegin) {
  core::SmallVector<int, 8> v = {2, 3};
  v.insert(v.cbegin(), 1);
  EXPECT_EQ(v.size(), 3);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
}

TEST(SmallVector, InsertAtMiddle) {
  core::SmallVector<int, 8> v = {1, 3};
  v.insert(v.cbegin() + 1, 2);
  EXPECT_EQ(v.size(), 3);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
}

TEST(SmallVector, InsertAtEnd) {
  core::SmallVector<int, 8> v = {1, 2};
  v.insert(v.cend(), 3);
  EXPECT_EQ(v.size(), 3);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
}

TEST(SmallVector, InsertOverCapacity) {
  core::SmallVector<int, 2> v = {1, 2};
  EXPECT_THROW(v.insert(v.cbegin(), 0), std::runtime_error);
}

TEST(SmallVector, InsertRvalue) {
  core::SmallVector<std::string, 8> v;
  v.push_back("b");
  v.insert(v.cbegin(), std::string("a"));
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v[0], "a");
  EXPECT_EQ(v[1], "b");
}

// --- erase ---

TEST(SmallVector, EraseAtBegin) {
  core::SmallVector<int, 8> v = {1, 2, 3};
  v.erase(v.begin());
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v[0], 2);
  EXPECT_EQ(v[1], 3);
}

TEST(SmallVector, EraseAtMiddle) {
  core::SmallVector<int, 8> v = {1, 2, 3};
  v.erase(v.begin() + 1);
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 3);
}

TEST(SmallVector, EraseAtEnd) {
  core::SmallVector<int, 8> v = {1, 2, 3};
  v.erase(v.begin() + 2);
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
}

TEST(SmallVector, EraseSingleElement) {
  core::SmallVector<int, 4> v = {42};
  v.erase(v.begin());
  EXPECT_EQ(v.size(), 0);
  EXPECT_TRUE(v.is_empty());
}

TEST(SmallVector, EraseWithConstIterator) {
  core::SmallVector<int, 8> v = {1, 2, 3};
  v.erase(v.cbegin() + 1);
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 3);
}

// --- Iterators ---

TEST(SmallVector, BeginEnd) {
  core::SmallVector<int, 8> v = {1, 2, 3};
  auto it = v.begin();
  EXPECT_EQ(*it, 1);
  ++it;
  EXPECT_EQ(*it, 2);
  ++it;
  EXPECT_EQ(*it, 3);
  ++it;
  EXPECT_EQ(it, v.end());
}

TEST(SmallVector, CbeginCend) {
  core::SmallVector<int, 8> v = {10, 20};
  auto it = v.cbegin();
  EXPECT_EQ(*it, 10);
  ++it;
  EXPECT_EQ(*it, 20);
  ++it;
  EXPECT_EQ(it, v.cend());
}

TEST(SmallVector, RangeFor) {
  core::SmallVector<int, 8> v = {1, 2, 3};
  int sum = 0;
  for (auto val : v) {
    sum += val;
  }
  EXPECT_EQ(sum, 6);
}

TEST(SmallVector, ConstIteration) {
  const core::SmallVector<int, 4> v = {5, 10, 15};
  int sum = 0;
  for (const auto& val : v) {
    sum += val;
  }
  EXPECT_EQ(sum, 30);
}

// --- Comparison (regression tests for operator== fix) ---

TEST(SmallVector, EqualSameContent) {
  core::SmallVector<int, 4> a = {1, 2, 3};
  core::SmallVector<int, 4> b = {1, 2, 3};
  EXPECT_TRUE(a == b);
  EXPECT_FALSE(a != b);
}

TEST(SmallVector, EqualDifferentContent) {
  core::SmallVector<int, 4> a = {1, 2, 3};
  core::SmallVector<int, 4> b = {1, 2, 4};
  EXPECT_FALSE(a == b);
  EXPECT_TRUE(a != b);
}

TEST(SmallVector, EqualDifferentSizes) {
  core::SmallVector<int, 4> a = {1, 2};
  core::SmallVector<int, 4> b = {1, 2, 3};
  EXPECT_FALSE(a == b);
  EXPECT_TRUE(a != b);
}

TEST(SmallVector, EqualAfterDifferentHistory) {
  core::SmallVector<int, 4> a = {1, 2};

  core::SmallVector<int, 4> b = {1, 2, 99};
  b.pop_back();

  EXPECT_EQ(a.size(), b.size());
  EXPECT_TRUE(a == b);
}

TEST(SmallVector, EqualEmptyVectors) {
  core::SmallVector<int, 4> a;
  core::SmallVector<int, 4> b;
  EXPECT_TRUE(a == b);
}

// --- constexpr tests ---

consteval int ConstexprBasicOps() {
  core::SmallVector<int, 8> v;
  v.push_back(10);
  v.push_back(20);
  v.push_back(30);
  if (v.size() != 3) return -1;
  if (v[0] != 10) return -2;
  if (v.front() != 10) return -3;
  if (v.back() != 30) return -4;
  v.pop_back();
  if (v.size() != 2) return -5;
  if (v.back() != 20) return -6;
  v.clear();
  if (v.size() != 0) return -7;
  if (!v.is_empty()) return -8;
  return 0;
}
static_assert(ConstexprBasicOps() == 0);

consteval int ConstexprInitList() {
  core::SmallVector<int, 4> v = {1, 2, 3};
  if (v.size() != 3) return -1;
  if (v[1] != 2) return -2;
  return 0;
}
static_assert(ConstexprInitList() == 0);

consteval bool ConstexprEquality() {
  core::SmallVector<int, 4> a = {1, 2, 3};
  core::SmallVector<int, 4> b = {1, 2, 3};
  return a == b;
}
static_assert(ConstexprEquality());

consteval int ConstexprResize() {
  core::SmallVector<int, 8> v = {1, 2, 3};
  v.resize(5);
  if (v.size() != 5) return -1;
  if (v[3] != 0) return -2;
  v.resize(2);
  if (v.size() != 2) return -3;
  if (v[0] != 1) return -4;
  return 0;
}
static_assert(ConstexprResize() == 0);

consteval int ConstexprInsertAndErase() {
  core::SmallVector<int, 8> v = {1, 3};
  v.insert(v.cbegin() + 1, 2);
  if (v.size() != 3) return -1;
  if (v[1] != 2) return -2;
  v.erase(v.cbegin());
  if (v.size() != 2) return -3;
  if (v[0] != 2) return -4;
  return 0;
}
static_assert(ConstexprInsertAndErase() == 0);

// --- Non-trivial types ---

TEST(SmallVector, StringPushBack) {
  core::SmallVector<std::string, 4> v;
  v.push_back("hello");
  v.push_back("world");
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v[0], "hello");
  EXPECT_EQ(v[1], "world");
}

TEST(SmallVector, StringClearAndReuse) {
  core::SmallVector<std::string, 4> v;
  v.push_back("alpha");
  v.push_back("beta");
  v.clear();
  EXPECT_TRUE(v.is_empty());
  v.push_back("gamma");
  EXPECT_EQ(v.size(), 1);
  EXPECT_EQ(v[0], "gamma");
}

TEST(SmallVector, MoveOnlyPushBack) {
  core::SmallVector<MoveOnly, 4> v;
  v.push_back(MoveOnly(10));
  v.push_back(MoveOnly(20));
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v[0].value, 10);
  EXPECT_EQ(v[1].value, 20);
}

TEST(SmallVector, MoveOnlyErase) {
  core::SmallVector<MoveOnly, 4> v;
  v.push_back(MoveOnly(1));
  v.push_back(MoveOnly(2));
  v.push_back(MoveOnly(3));
  v.erase(v.begin() + 1);
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v[0].value, 1);
  EXPECT_EQ(v[1].value, 3);
}
