#include <gtest/gtest.h>
#include <Player.hpp>
#include <weapon/Gun.hpp>

static Player make_player(Vec2 pos = Vec2(100, 100), int hp = 100) {
    return Player(pos, hp, 300.0f, std::make_unique<Gun>());
}

TEST(Player, AliveOnCreation) {
    auto player = make_player();
    EXPECT_FALSE(player.is_dead());
    EXPECT_EQ(player.hp, 100);
}

TEST(Player, TakeDamageReducesHp) {
    auto player = make_player();
    player.take_damage(30);
    EXPECT_EQ(player.hp, 70);
}

TEST(Player, TakeDamageLethal) {
    auto player = make_player();
    player.take_damage(100);
    EXPECT_TRUE(player.is_dead());
}

TEST(Player, TakeDamageOverkillStopsAtZero) {
    auto player = make_player();
    player.take_damage(200);
    EXPECT_LE(player.hp, 0);
    EXPECT_TRUE(player.is_dead());
}

TEST(Player, KillSetsDeadAndHpZero) {
    auto player = make_player();
    player.kill();
    EXPECT_TRUE(player.is_dead());
    EXPECT_EQ(player.hp, 0);
}

TEST(Player, MaxHpStoredCorrectly) {
    auto player = make_player(Vec2(0, 0), 80);
    EXPECT_EQ(player.max_hp, 80);
}

TEST(Player, OnWallCollisionRestoresPosition) {
    auto player = make_player(Vec2(100, 200));
    player.update(0.016f);          // prev_position_ = (100, 200) после update
    player.position = Vec2(999, 999);
    player.on_wall_collision();
    EXPECT_NEAR(player.position.x, 100.0, 1.0);
    EXPECT_NEAR(player.position.y, 200.0, 1.0);
}

TEST(Player, TakeTileDamageCooldown) {
    auto player = make_player();
    player.take_tile_damage(10);
    EXPECT_EQ(player.hp, 90);
    // повторный урон в ту же frame — не проходит из-за cooldown
    player.take_tile_damage(10);
    EXPECT_EQ(player.hp, 90);
}

TEST(Player, TakeTileDamageAfterCooldown) {
    auto player = make_player();
    player.take_tile_damage(10);
    EXPECT_EQ(player.hp, 90);
    // тикаем cooldown достаточно времени (cooldown = 0.5s)
    player.update(0.6f);
    player.take_tile_damage(10);
    EXPECT_EQ(player.hp, 80);
}
