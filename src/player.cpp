#include "../libraries/player.hpp"
#include "map/map.hpp"
#include "weapon/weapon.hpp"

Player::Player(Vec2 pos, int hp, float speed, std::unique_ptr<Weapon> weapon)
    : Entity(pos, 0, 32), hp_(hp), speed_(speed), weapon_(std::move(weapon)) {}

void Player::move(float timediff) {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) position.y -= speed_ * timediff;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) position.y += speed_ * timediff;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) position.x -= speed_ * timediff;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) position.x += speed_ * timediff;
}

void Player::set_mouse(sf::Vector2i mouse) {
  mouse_pos_ = Vec2(mouse.x, mouse.y);
}

Vec2 Player::dir() const {
  Vec2 center(position.x + 63, position.y + 39);
  return Vec2(mouse_pos_.x - center.x, mouse_pos_.y - center.y);
}

void Player::attack(Map& map) {
  if (!weapon_) return;
  weapon_->shoot(map, *this, 10); // пока damage будет 10
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
  position.x -= position.x * 0.1f;
  position.y -= position.y * 0.1f;
}

void Player::take_damage(int damage) {
  if (hp_ > 0) {
    hp_ -= damage;
  }
}

//есть в энтити убрать нельзя но по сути копипаст
void Player::update(float timediff) {
  move(timediff);
}

