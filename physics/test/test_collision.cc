#include <gtest/gtest.h>

#include "collision.h"
#include "world.h"

// All collision tests use the global physics world — reset before each test.
class CollisionTest : public ::testing::Test {
 protected:
  void SetUp() override { physics::ResetWorld(); }
};

// --- Contact generation ---

TEST_F(CollisionTest, CircleCircleOverlapping) {
  auto idx_a = physics::AddBody(1.f);
  physics::body_at(idx_a).position = {0.f, 0.f};
  auto col_a = physics::AddColliderToBody(idx_a, {0.f, 0.f}, 1.f,
                                          physics::Circle(10.f),
                                          physics::ShapeType::Circle, false);

  auto idx_b = physics::AddBody(1.f);
  physics::body_at(idx_b).position = {15.f, 0.f};  // dist=15 < r1+r2=20
  auto col_b = physics::AddColliderToBody(idx_b, {0.f, 0.f}, 1.f,
                                          physics::Circle(10.f),
                                          physics::ShapeType::Circle, false);

  auto contact = physics::GenerateContact(col_a, col_b);
  ASSERT_TRUE(contact.has_value());
  EXPECT_NEAR(contact->penetration, 5.f, 1e-4f);
}

TEST_F(CollisionTest, CircleCircleNotOverlapping) {
  auto idx_a = physics::AddBody(1.f);
  physics::body_at(idx_a).position = {0.f, 0.f};
  auto col_a = physics::AddColliderToBody(idx_a, {0.f, 0.f}, 1.f,
                                          physics::Circle(10.f),
                                          physics::ShapeType::Circle, false);

  auto idx_b = physics::AddBody(1.f);
  physics::body_at(idx_b).position = {25.f, 0.f};  // dist=25 > r1+r2=20
  auto col_b = physics::AddColliderToBody(idx_b, {0.f, 0.f}, 1.f,
                                          physics::Circle(10.f),
                                          physics::ShapeType::Circle, false);

  auto contact = physics::GenerateContact(col_a, col_b);
  EXPECT_FALSE(contact.has_value());
}

TEST_F(CollisionTest, AabbAabbOverlapping) {
  auto idx_a = physics::AddBody(1.f);
  physics::body_at(idx_a).position = {0.f, 0.f};
  auto col_a = physics::AddColliderToBody(idx_a, {0.f, 0.f}, 1.f,
                                          physics::AABB({20.f, 20.f}),
                                          physics::ShapeType::AABB, false);

  auto idx_b = physics::AddBody(1.f);
  physics::body_at(idx_b).position = {15.f, 0.f};  // overlap by 5 in x
  auto col_b = physics::AddColliderToBody(idx_b, {0.f, 0.f}, 1.f,
                                          physics::AABB({20.f, 20.f}),
                                          physics::ShapeType::AABB, false);

  auto contact = physics::GenerateContact(col_a, col_b);
  EXPECT_TRUE(contact.has_value());
}

TEST_F(CollisionTest, AabbAabbNotOverlapping) {
  auto idx_a = physics::AddBody(1.f);
  physics::body_at(idx_a).position = {0.f, 0.f};
  auto col_a = physics::AddColliderToBody(idx_a, {0.f, 0.f}, 1.f,
                                          physics::AABB({20.f, 20.f}),
                                          physics::ShapeType::AABB, false);

  auto idx_b = physics::AddBody(1.f);
  physics::body_at(idx_b).position = {25.f, 0.f};  // no overlap
  auto col_b = physics::AddColliderToBody(idx_b, {0.f, 0.f}, 1.f,
                                          physics::AABB({20.f, 20.f}),
                                          physics::ShapeType::AABB, false);

  auto contact = physics::GenerateContact(col_a, col_b);
  EXPECT_FALSE(contact.has_value());
}

TEST_F(CollisionTest, CircleAabbOverlapping) {
  auto idx_a = physics::AddBody(1.f);
  physics::body_at(idx_a).position = {0.f, 0.f};
  auto col_a = physics::AddColliderToBody(idx_a, {0.f, 0.f}, 1.f,
                                          physics::Circle(10.f),
                                          physics::ShapeType::Circle, false);

  auto idx_b = physics::AddBody(1.f);
  physics::body_at(idx_b).position = {15.f, 0.f};
  auto col_b = physics::AddColliderToBody(idx_b, {0.f, 0.f}, 1.f,
                                          physics::AABB({20.f, 20.f}),
                                          physics::ShapeType::AABB, false);

  auto contact = physics::GenerateContact(col_a, col_b);
  EXPECT_TRUE(contact.has_value());
}

TEST_F(CollisionTest, CircleAabbNotOverlapping) {
  auto idx_a = physics::AddBody(1.f);
  physics::body_at(idx_a).position = {0.f, 0.f};
  auto col_a = physics::AddColliderToBody(idx_a, {0.f, 0.f}, 1.f,
                                          physics::Circle(10.f),
                                          physics::ShapeType::Circle, false);

  auto idx_b = physics::AddBody(1.f);
  physics::body_at(idx_b).position = {30.f, 0.f};
  auto col_b = physics::AddColliderToBody(idx_b, {0.f, 0.f}, 1.f,
                                          physics::AABB({20.f, 20.f}),
                                          physics::ShapeType::AABB, false);

  auto contact = physics::GenerateContact(col_a, col_b);
  EXPECT_FALSE(contact.has_value());
}

// --- Collision resolution ---

TEST_F(CollisionTest, ResolveDynamicVsDynamicEqualMassElastic) {
  // Equal-mass elastic collision: velocities exchange.
  auto idx_a = physics::AddBody(1.f);
  physics::body_at(idx_a).position = {0.f, 0.f};
  physics::body_at(idx_a).Velocity({50.f, 0.f});
  auto col_a = physics::AddColliderToBody(idx_a, {0.f, 0.f}, 1.f,
                                          physics::Circle(10.f),
                                          physics::ShapeType::Circle, false);

  auto idx_b = physics::AddBody(1.f);
  physics::body_at(idx_b).position = {15.f, 0.f};
  physics::body_at(idx_b).Velocity({0.f, 0.f});
  auto col_b = physics::AddColliderToBody(idx_b, {0.f, 0.f}, 1.f,
                                          physics::Circle(10.f),
                                          physics::ShapeType::Circle, false);

  auto contact = physics::GenerateContact(col_a, col_b);
  ASSERT_TRUE(contact.has_value());
  physics::ResolveCollision(*contact, 1.f);

  EXPECT_NEAR(physics::body_at(idx_a).velocity().x, 0.f, 1e-4f);
  EXPECT_NEAR(physics::body_at(idx_b).velocity().x, 50.f, 1e-4f);
}

TEST_F(CollisionTest, ResolveStaticVsDynamicStaticUnaffected) {
  // Dynamic ball moving toward a static wall: wall stays still, ball bounces.
  auto idx_wall = physics::AddStaticBody();
  physics::body_at(idx_wall).position = {0.f, 0.f};
  auto col_wall = physics::AddColliderToBody(idx_wall, {0.f, 0.f}, 1.f,
                                             physics::Circle(10.f),
                                             physics::ShapeType::Circle, false);

  auto idx_ball = physics::AddBody(1.f);
  physics::body_at(idx_ball).position = {15.f, 0.f};
  physics::body_at(idx_ball).Velocity({-50.f, 0.f});
  auto col_ball = physics::AddColliderToBody(idx_ball, {0.f, 0.f}, 1.f,
                                             physics::Circle(10.f),
                                             physics::ShapeType::Circle, false);

  auto contact = physics::GenerateContact(col_wall, col_ball);
  ASSERT_TRUE(contact.has_value());
  physics::ResolveCollision(*contact, 1.f);

  EXPECT_EQ(physics::body_at(idx_wall).velocity().x, 0.f);
  EXPECT_EQ(physics::body_at(idx_wall).velocity().y, 0.f);
  EXPECT_EQ(physics::body_at(idx_wall).position.x, 0.f);
  EXPECT_GT(physics::body_at(idx_ball).velocity().x, 0.f);  // bounced back
}

TEST_F(CollisionTest, ResolveBothStaticNothingMoves) {
  auto idx_a = physics::AddStaticBody();
  physics::body_at(idx_a).position = {0.f, 0.f};
  auto col_a = physics::AddColliderToBody(idx_a, {0.f, 0.f}, 1.f,
                                          physics::Circle(10.f),
                                          physics::ShapeType::Circle, false);

  auto idx_b = physics::AddStaticBody();
  physics::body_at(idx_b).position = {15.f, 0.f};
  auto col_b = physics::AddColliderToBody(idx_b, {0.f, 0.f}, 1.f,
                                          physics::Circle(10.f),
                                          physics::ShapeType::Circle, false);

  auto contact = physics::GenerateContact(col_a, col_b);
  ASSERT_TRUE(contact.has_value());
  physics::ResolveCollision(*contact, 1.f);  // should be a no-op

  EXPECT_EQ(physics::body_at(idx_a).position.x, 0.f);
  EXPECT_EQ(physics::body_at(idx_b).position.x, 15.f);
}

TEST_F(CollisionTest, ResolveContactNormalPointsFromAToB) {
  auto idx_a = physics::AddBody(1.f);
  physics::body_at(idx_a).position = {0.f, 0.f};
  auto col_a = physics::AddColliderToBody(idx_a, {0.f, 0.f}, 1.f,
                                          physics::Circle(10.f),
                                          physics::ShapeType::Circle, false);

  auto idx_b = physics::AddBody(1.f);
  physics::body_at(idx_b).position = {15.f, 0.f};
  auto col_b = physics::AddColliderToBody(idx_b, {0.f, 0.f}, 1.f,
                                          physics::Circle(10.f),
                                          physics::ShapeType::Circle, false);

  auto contact = physics::GenerateContact(col_a, col_b);
  ASSERT_TRUE(contact.has_value());

  // Normal must point from A toward B — x > 0 when B is to the right of A.
  EXPECT_GT(contact->normal.x, 0.f);
  EXPECT_NEAR(contact->normal.y, 0.f, 1e-5f);
}
