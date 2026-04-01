#include <gtest/gtest.h>

#include "body.h"

// Pure Body unit tests — no global world state needed.

TEST(Body, DynamicBodyMovesWithVelocity) {
  physics::Body body(1.f);
  body.Velocity({10.f, 0.f});
  body.Tick(1.f);
  EXPECT_NEAR(body.position.x, 10.f, 1e-5f);
  EXPECT_NEAR(body.position.y, 0.f, 1e-5f);
}

TEST(Body, StaticBodyDoesNotMove) {
  physics::Body body(1.f);
  body.is_static = true;
  body.Velocity({10.f, 0.f});
  body.Tick(1.f);
  EXPECT_EQ(body.position.x, 0.f);
  EXPECT_EQ(body.position.y, 0.f);
}

TEST(Body, ForceAcceleratesBody) {
  physics::Body body(1.f);  // mass = 1, a = F/m = F
  body.AddForce({10.f, 0.f});
  body.Tick(1.f);
  EXPECT_NEAR(body.velocity().x, 10.f, 1e-5f);
}

TEST(Body, ForceClearedAfterTick) {
  physics::Body body(1.f);
  body.AddForce({10.f, 0.f});
  body.Tick(1.f);
  // Second tick with no force — velocity stays constant.
  body.Tick(1.f);
  EXPECT_NEAR(body.velocity().x, 10.f, 1e-5f);
}

TEST(Body, StaticBodyIgnoresForce) {
  physics::Body body(1.f);
  body.is_static = true;
  body.AddForce({100.f, 0.f});
  body.Tick(1.f);
  EXPECT_EQ(body.velocity().x, 0.f);
  EXPECT_EQ(body.position.x, 0.f);
}
