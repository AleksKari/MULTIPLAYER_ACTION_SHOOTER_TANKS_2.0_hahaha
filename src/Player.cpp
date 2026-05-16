#include <Player.hpp>
#include <map/Map.hpp>
#include <weapon/Weapon.hpp>
#include <weapon/Gun.hpp>
#include <iostream>

Player::Player(Vec2 pos, int hp, float speed, std::unique_ptr<Weapon> weapon)
    : Entity(pos, 0, TILESIZE), hp(hp), max_hp(hp), speed_(speed), weapon_(std::move(weapon)) {}

void Player::move(float timediff) {
  Vec2 dir(
    sf::Keyboard::isKeyPressed(sf::Keyboard::D) - sf::Keyboard::isKeyPressed(sf::Keyboard::A),
    sf::Keyboard::isKeyPressed(sf::Keyboard::S) - sf::Keyboard::isKeyPressed(sf::Keyboard::W)
  );
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

bool Player::is_dead() const {
  return hp <= 0;
}

//вроде так но если что снос alive
void Player::kill() {
  hp = 0;
}

//не даем застрять откатывает координаты
void Player::on_wall_collision() {
  position = prev_position_;
}

void Player::take_damage(int damage) {
  if (hp > 0) {
    hp -= damage;
  }
}

void Player::update(float timediff) {
  prev_position_ = position;
  move(timediff);
  cornrotate = dir().angle();

  sprite_.setPosition(position.x + size_ / 2.0f, position.y + size_ / 2.0f);
  sprite_.setRotation(cornrotate);
}

void Player::loadSkin(const std::string& path) {
  if (!texture_.loadFromFile(path)) {
      std::cerr << "Error loading skin: " << path << "\n";
      return;
  }
  sprite_.setTexture(texture_); 
  sf::FloatRect bounds = sprite_.getLocalBounds();
  
  sprite_.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
  sprite_.setScale(
      static_cast<float>(size_) / bounds.width,
      static_cast<float>(size_) / bounds.height
  );
}

