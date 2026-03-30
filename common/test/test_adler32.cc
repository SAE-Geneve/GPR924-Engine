//
// Created by unite on 19.03.2026.
//

#include "network/adler32.h"
#include "gtest/gtest.h"

TEST(Adler32, Order) {
  common::Adler32 v1;
  v1.Add(2);
  v1.Add(1);

  common::Adler32 v2;
  v2.Add(1);
  v2.Add(2);

  EXPECT_NE(v1.value(), v2.value());
}

TEST(Adler32, IsNotChecksum) {
  common::Adler32 v1;
  v1.Add(2);
  v1.Add(1);

  EXPECT_NE(v1.value(), 3);
}