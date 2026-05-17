#include <gtest/gtest.h>
#include <math/Vec2.h>
#include <cmath>

TEST(Vec2, DefaultConstructorZero) {
    Vec2 v;
    EXPECT_DOUBLE_EQ(v.x, 0.0);
    EXPECT_DOUBLE_EQ(v.y, 0.0);
}

TEST(Vec2, Constructor) {
    Vec2 v(3.0, 4.0);
    EXPECT_DOUBLE_EQ(v.x, 3.0);
    EXPECT_DOUBLE_EQ(v.y, 4.0);
}

TEST(Vec2, Addition) {
    Vec2 result = Vec2(1.0, 2.0) + Vec2(3.0, 4.0);
    EXPECT_DOUBLE_EQ(result.x, 4.0);
    EXPECT_DOUBLE_EQ(result.y, 6.0);
}

TEST(Vec2, Subtraction) {
    Vec2 result = Vec2(5.0, 3.0) - Vec2(2.0, 1.0);
    EXPECT_DOUBLE_EQ(result.x, 3.0);
    EXPECT_DOUBLE_EQ(result.y, 2.0);
}

TEST(Vec2, ScalarMultiply) {
    Vec2 result = Vec2(2.0, 3.0) * 2.0;
    EXPECT_DOUBLE_EQ(result.x, 4.0);
    EXPECT_DOUBLE_EQ(result.y, 6.0);
}

TEST(Vec2, Length) {
    EXPECT_DOUBLE_EQ(Vec2(3.0, 4.0).length(), 5.0);
}

TEST(Vec2, LengthZeroVector) {
    EXPECT_DOUBLE_EQ(Vec2(0.0, 0.0).length(), 0.0);
}

TEST(Vec2, NormalizedLength) {
    Vec2 n = Vec2(3.0, 4.0).normilized();
    EXPECT_NEAR(n.length(), 1.0, 1e-9);
}

TEST(Vec2, NormalizedDirection) {
    Vec2 n = Vec2(3.0, 4.0).normilized();
    EXPECT_NEAR(n.x, 0.6, 1e-9);
    EXPECT_NEAR(n.y, 0.8, 1e-9);
}

TEST(Vec2, NormalizedZeroVectorReturnsZero) {
    Vec2 n = Vec2(0.0, 0.0).normilized();
    EXPECT_DOUBLE_EQ(n.x, 0.0);
    EXPECT_DOUBLE_EQ(n.y, 0.0);
}

TEST(Vec2, AngleRight) {
    EXPECT_NEAR(Vec2(1.0, 0.0).angle(), 0.0, 1e-9);
}

TEST(Vec2, AngleDown) {
    EXPECT_NEAR(Vec2(0.0, 1.0).angle(), M_PI / 2.0, 1e-9);
}

TEST(Vec2, AngleLeft) {
    EXPECT_NEAR(std::abs(Vec2(-1.0, 0.0).angle()), M_PI, 1e-9);
}

TEST(Vec2, AngleConstructor) {
    Vec2 v(0.0);  // угол 0 = (1, 0)
    EXPECT_NEAR(v.x, 1.0, 1e-9);
    EXPECT_NEAR(v.y, 0.0, 1e-9);
}
