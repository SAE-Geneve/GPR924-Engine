#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "contact_listener.h"
#include "world.h"

class MockContactListener : public listeners::ContactListener {
 public:
  MOCK_METHOD(void, OnTriggerEnter, (physics::CollidersPair), (override));
  MOCK_METHOD(void, OnTriggerExit, (physics::CollidersPair), (override));
  MOCK_METHOD(void, OnColliderEnter, (physics::CollidersPair), (override));
  MOCK_METHOD(void, OnColliderExit, (physics::CollidersPair), (override));
};

// Resets world + sets bounds (needed to initialize the quadtree).
class WorldTest : public ::testing::Test {
 protected:
  void SetUp() override {
    physics::ResetWorld();
    physics::set_world_bounds(physics::AABB{{0.f, 0.f}, {1000.f, 1000.f}});
  }
  void TearDown() override { physics::set_contact_listener(nullptr); }
};

// --- World tick ---

TEST_F(WorldTest, DynamicBodyMovesAfterTick) {
  auto idx = physics::AddBody(1.f);
  physics::body_at(idx).position = {0.f, 0.f};
  physics::body_at(idx).Velocity({100.f, 0.f});

  physics::Tick(1.f);

  EXPECT_NEAR(physics::body_at(idx).position.x, 100.f, 1e-4f);
}

TEST_F(WorldTest, StaticBodyDoesNotMoveAfterTick) {
  auto idx = physics::AddStaticBody();
  physics::body_at(idx).position = {50.f, 50.f};

  physics::Tick(1.f);

  EXPECT_EQ(physics::body_at(idx).position.x, 50.f);
  EXPECT_EQ(physics::body_at(idx).position.y, 50.f);
}

// --- Contact listener ---

TEST_F(WorldTest, OnColliderEnterFiredOnOverlap) {
  testing::NiceMock<MockContactListener> listener;
  physics::set_contact_listener(&listener);

  auto idx_a = physics::AddBody(1.f);
  physics::body_at(idx_a).position = {100.f, 100.f};
  [[maybe_unused]] auto col_a = physics::AddColliderToBody(
      idx_a, {0.f, 0.f}, 1.f, physics::Circle(20.f),
      physics::ShapeType::Circle, false);

  auto idx_b = physics::AddBody(1.f);
  physics::body_at(idx_b).position = {120.f, 100.f};  // dist=20 < r1+r2=40
  [[maybe_unused]] auto col_b = physics::AddColliderToBody(
      idx_b, {0.f, 0.f}, 1.f, physics::Circle(20.f),
      physics::ShapeType::Circle, false);

  EXPECT_CALL(listener, OnColliderEnter(testing::_)).Times(1);

  physics::Tick(1.f / 60.f);
}

TEST_F(WorldTest, OnColliderExitFiredWhenSeparated) {
  testing::NiceMock<MockContactListener> listener;
  physics::set_contact_listener(&listener);

  auto idx_a = physics::AddBody(1.f);
  physics::body_at(idx_a).position = {100.f, 100.f};
  [[maybe_unused]] auto col_a = physics::AddColliderToBody(
      idx_a, {0.f, 0.f}, 1.f, physics::Circle(20.f),
      physics::ShapeType::Circle, false);

  auto idx_b = physics::AddBody(1.f);
  physics::body_at(idx_b).position = {120.f, 100.f};
  [[maybe_unused]] auto col_b = physics::AddColliderToBody(
      idx_b, {0.f, 0.f}, 1.f, physics::Circle(20.f),
      physics::ShapeType::Circle, false);

  EXPECT_CALL(listener, OnColliderEnter(testing::_)).Times(1);
  physics::Tick(1.f / 60.f);  // enter

  EXPECT_CALL(listener, OnColliderExit(testing::_)).Times(1);
  physics::body_at(idx_a).position = {0.f, 0.f};
  physics::body_at(idx_b).position = {500.f, 500.f};
  physics::Tick(1.f / 60.f);  // exit
}

TEST_F(WorldTest, OnTriggerEnterFiredForTriggerCollider) {
  testing::NiceMock<MockContactListener> listener;
  physics::set_contact_listener(&listener);

  auto idx_a = physics::AddBody(1.f);
  physics::body_at(idx_a).position = {100.f, 100.f};
  [[maybe_unused]] auto col_a = physics::AddColliderToBody(
      idx_a, {0.f, 0.f}, 1.f, physics::Circle(20.f),
      physics::ShapeType::Circle, true);  // trigger

  auto idx_b = physics::AddBody(1.f);
  physics::body_at(idx_b).position = {120.f, 100.f};
  [[maybe_unused]] auto col_b = physics::AddColliderToBody(
      idx_b, {0.f, 0.f}, 1.f, physics::Circle(20.f),
      physics::ShapeType::Circle, false);

  EXPECT_CALL(listener, OnTriggerEnter(testing::_)).Times(1);
  EXPECT_CALL(listener, OnColliderEnter(testing::_)).Times(0);

  physics::Tick(1.f / 60.f);
}

TEST_F(WorldTest, TriggerDoesNotResolveCollision) {
  // A trigger collider must not apply a physics impulse.
  testing::NiceMock<MockContactListener> listener;
  physics::set_contact_listener(&listener);

  auto idx_ball = physics::AddBody(1.f);
  physics::body_at(idx_ball).position = {100.f, 100.f};
  physics::body_at(idx_ball).Velocity({-100.f, 0.f});
  [[maybe_unused]] auto col_ball = physics::AddColliderToBody(
      idx_ball, {0.f, 0.f}, 1.f, physics::Circle(20.f),
      physics::ShapeType::Circle, true);  // trigger

  auto idx_wall = physics::AddStaticBody();
  physics::body_at(idx_wall).position = {120.f, 100.f};
  [[maybe_unused]] auto col_wall = physics::AddColliderToBody(
      idx_wall, {0.f, 0.f}, 1.f, physics::Circle(20.f),
      physics::ShapeType::Circle, false);

  physics::Tick(1.f / 60.f);

  // Velocity direction must not have been flipped — no impulse applied.
  EXPECT_LT(physics::body_at(idx_ball).velocity().x, 0.f);
}
