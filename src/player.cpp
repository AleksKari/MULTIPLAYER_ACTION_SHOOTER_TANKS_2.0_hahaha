#include <player.hpp>
#include "map/map.hpp"
#include "weapon/weapon.hpp"
#include "weapon/gun.hpp"

Player::Player(Vec2 pos, int hp, float speed, std::unique_ptr<Weapon> weapon)
    : Entity(pos, 0, 32), hp_(hp), max_hp_(hp), speed_(speed), weapon_(std::move(weapon)) {}

void Player::move(float timediff) {
  Vec2 dir(0, 0);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) dir.y -= 1;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) dir.y += 1;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) dir.x -= 1;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) dir.x += 1;
  position = position + dir.normilized() * speed_ * timediff;
}

void Player::set_mouse(sf::Vector2i mouse) {
  mouse_pos_ = Vec2(mouse.x, mouse.y);
}

Vec2 Player::dir() const {
  Vec2 center(position.x + 16, position.y + 16);
  return Vec2(mouse_pos_.x - center.x, mouse_pos_.y - center.y);
}

void Player::attack(Map& map) {
  if (!weapon_) return;
  weapon_->shoot(map, *this, 10); // пока damage будет 10
  if (weapon_->no_ammo()) weapon_ = std::make_unique<Gun>();
}

void Player::set_weapon(std::unique_ptr<Weapon> weapon) {
  weapon_ = std::move(weapon);
}

bool Player::isDead() const {
  return hp_ <= 0;
}

//вроде так но если что снос alive
void Player::kill() {
  hp_ = 0;
}

//не даем застрять откатывает координаты
void Player::on_wall_collision() {
  position = prev_position_;
}

void Player::take_damage(int damage) {
  if (hp_ > 0) {
    hp_ -= damage;
  }
}

//есть в энтити убрать нельзя но по сути копипаст
void Player::update(float timediff) {
  prev_position_ = position;
  move(timediff);
  cornrotate = dir().angle();
}

