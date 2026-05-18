#include <gtest/gtest.h>

#include <collision/Collision.hpp>

// Collision::entity_projectile — столкновение двух кругов
// pos — это левый верхний угол, центр = pos + size/2

TEST(CollisionEntityProjectile, SamePosCollides) {
  EXPECT_TRUE(Collision::entity_projectile(Vec2(0, 0), 6, Vec2(0, 0), 32));
}

TEST(CollisionEntityProjectile, FarApartNoCollision) {
  EXPECT_FALSE(
      Collision::entity_projectile(Vec2(0, 0), 6, Vec2(1000, 1000), 32));
}

// Центры: proj_center = pos + size/2, entity_center = pos + size/2
// Сумма радиусов для size=6 и size=32: 3 + 16 = 19

TEST(CollisionEntityProjectile, JustOutside) {
  // proj center=(3,3), entity center=(22,3) — расстояние=19, НЕ пересекаются
  // (строго <)
  EXPECT_FALSE(Collision::entity_projectile(Vec2(0, 0), 6, Vec2(6, -13), 32));
}

TEST(CollisionEntityProjectile, JustOverlapping) {
  // proj center=(3,3), entity center=(21,3) — расстояние=18 < 19 — пересекаются
  EXPECT_TRUE(Collision::entity_projectile(Vec2(0, 0), 6, Vec2(5, -13), 32));
}

TEST(CollisionEntityProjectile, VerticalOverlap) {
  // proj center=(3,3), entity center=(3,21) — расстояние=18 < 19 — пересекаются
  EXPECT_TRUE(Collision::entity_projectile(Vec2(0, 0), 6, Vec2(-13, 5), 32));
}

TEST(CollisionEntityProjectile, DiagonalOverlap) {
  // proj center=(3,3), entity center=(3+9,3+9)=(12,12) — расстояние≈12.7 < 19 —
  // пересекаются
  EXPECT_TRUE(Collision::entity_projectile(Vec2(0, 0), 6, Vec2(-4, -4), 32));
}

TEST(CollisionEntityProjectile, DiagonalNoCollision) {
  // proj center=(3,3), entity center=(17,17) — расстояние≈19.8 > 19 — НЕ
  // пересекаются
  EXPECT_FALSE(Collision::entity_projectile(Vec2(0, 0), 6, Vec2(1, 1), 32));
}
