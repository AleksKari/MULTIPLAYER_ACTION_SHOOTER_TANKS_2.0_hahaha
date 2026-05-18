#include <gtest/gtest.h>

#include <Projectile.hpp>

TEST(Projectile, AliveOnCreation) {
  Projectile projectile(Vec2(0, 0), Vec2(100, 0), 10, 6, 1);
  EXPECT_FALSE(projectile.is_dead());
}

TEST(Projectile, KillMakesDead) {
  Projectile projectile(Vec2(0, 0), Vec2(100, 0), 10, 6, 1);
  projectile.kill();
  EXPECT_TRUE(projectile.is_dead());
}

TEST(Projectile, TakeDamageLethal) {
  Projectile projectile(Vec2(0, 0), Vec2(100, 0), 10, 6, 1);
  projectile.take_damage(1);
  EXPECT_TRUE(projectile.is_dead());
}

TEST(Projectile, TakeDamagePartialHp) {
  Projectile projectile(Vec2(0, 0), Vec2(100, 0), 10, 6, 3);
  projectile.take_damage(1);
  EXPECT_FALSE(projectile.is_dead());
  projectile.take_damage(1);
  EXPECT_FALSE(projectile.is_dead());
  projectile.take_damage(1);
  EXPECT_TRUE(projectile.is_dead());
}

TEST(Projectile, GetDamage) {
  Projectile projectile(Vec2(0, 0), Vec2(100, 0), 15, 6, 1);
  EXPECT_EQ(projectile.get_damage(), 15);
}

TEST(Projectile, LifetimeNonNegative) {
  Projectile projectile(Vec2(0, 0), Vec2(100, 0), 10, 6, 1);
  EXPECT_GE(projectile.lifetime(), 0.0);
}

TEST(Projectile, NoRicochetByDefault) {
  Projectile projectile(Vec2(0, 0), Vec2(100, 0), 10, 6, 1);
  EXPECT_FALSE(projectile.can_ricochet());
}

TEST(Projectile, PositionAfterUpdate) {
  Projectile projectile(Vec2(0, 0), Vec2(100, 0), 10, 6, 1);
  projectile.update(1.0f);
  EXPECT_NEAR(projectile.position.cord_x, 100.0, 1e-3);
  EXPECT_NEAR(projectile.position.cord_y, 0.0, 1e-3);
}

TEST(Projectile, SizeStored) {
  Projectile projectile(Vec2(0, 0), Vec2(0, 0), 10, 8, 1);
  EXPECT_EQ(projectile.size, 8);
}
