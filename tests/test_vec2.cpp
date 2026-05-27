#include <gtest/gtest.h>
#include <math/Vec2.h>

#include <cmath>

TEST(Vec2, DefaultConstructorZero) {
  Vec2 v;
  EXPECT_DOUBLE_EQ(v.cord_x, 0.0);
  EXPECT_DOUBLE_EQ(v.cord_y, 0.0);
}

TEST(Vec2, Constructor) {
  Vec2 v(3.0, 4.0);
  EXPECT_DOUBLE_EQ(v.cord_x, 3.0);
  EXPECT_DOUBLE_EQ(v.cord_y, 4.0);
}

TEST(Vec2, Addition) {
  Vec2 result = Vec2(1.0, 2.0) + Vec2(3.0, 4.0);
  EXPECT_DOUBLE_EQ(result.cord_x, 4.0);
  EXPECT_DOUBLE_EQ(result.cord_y, 6.0);
}

TEST(Vec2, Subtraction) {
  Vec2 result = Vec2(5.0, 3.0) - Vec2(2.0, 1.0);
  EXPECT_DOUBLE_EQ(result.cord_x, 3.0);
  EXPECT_DOUBLE_EQ(result.cord_y, 2.0);
}

TEST(Vec2, ScalarMultiply) {
  Vec2 result = Vec2(2.0, 3.0) * 2.0;
  EXPECT_DOUBLE_EQ(result.cord_x, 4.0);
  EXPECT_DOUBLE_EQ(result.cord_y, 6.0);
}

TEST(Vec2, Length) { EXPECT_DOUBLE_EQ(Vec2(3.0, 4.0).Length(), 5.0); }

TEST(Vec2, LengthZeroVector) { EXPECT_DOUBLE_EQ(Vec2(0.0, 0.0).Length(), 0.0); }

TEST(Vec2, NormalizedLength) {
  Vec2 n = Vec2(3.0, 4.0).Normalized();
  EXPECT_NEAR(n.Length(), 1.0, 1e-9);
}

TEST(Vec2, NormalizedDirection) {
  Vec2 n = Vec2(3.0, 4.0).Normalized();
  EXPECT_NEAR(n.cord_x, 0.6, 1e-9);
  EXPECT_NEAR(n.cord_y, 0.8, 1e-9);
}

TEST(Vec2, NormalizedZeroVectorReturnsZero) {
  Vec2 n = Vec2(0.0, 0.0).Normalized();
  EXPECT_DOUBLE_EQ(n.cord_x, 0.0);
  EXPECT_DOUBLE_EQ(n.cord_y, 0.0);
}

TEST(Vec2, AngleRight) { EXPECT_NEAR(Vec2(1.0, 0.0).Angle(), 0.0, 1e-9); }

TEST(Vec2, AngleDown) { EXPECT_NEAR(Vec2(0.0, 1.0).Angle(), M_PI / 2.0, 1e-9); }

TEST(Vec2, AngleLeft) {
  EXPECT_NEAR(std::abs(Vec2(-1.0, 0.0).Angle()), M_PI, 1e-9);
}

TEST(Vec2, AngleConstructor) {
  Vec2 v(0.0);  // угол 0 = (1, 0)
  EXPECT_NEAR(v.cord_x, 1.0, 1e-9);
  EXPECT_NEAR(v.cord_y, 0.0, 1e-9);
}
