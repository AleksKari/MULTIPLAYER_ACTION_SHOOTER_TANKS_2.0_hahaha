#include <Player.hpp>
#include <map/Map.hpp>
#include <weapon/Weapon.hpp>
#include <weapon/Gun.hpp>
#include <iostream>

Player::Player(Vec2 pos, int hp, float speed, std::unique_ptr<Weapon> weapon)
    : Entity(pos, 0, TILESIZE), hp(hp), max_hp(hp), speed_(speed), weapon_(std::move(weapon)) {
  if (position.x > 900.0) {
    facing_dir_ = Vec2(-1.0, 0.0);
  }
}

void Player::move(float timediff) {
  Vec2 input_dir(
    static_cast<int>(input_d) - static_cast<int>(input_a),
    static_cast<int>(input_s) - static_cast<int>(input_w)
  );
  const Vec2 normalized = input_dir.normalized();
  if (normalized.length() > 0.0) {
    facing_dir_ = normalized;
  }
  position = position + normalized * speed_ * slow_coeff_ * timediff;
}

void Player::set_mouse(sf::Vector2i mouse) {
  mouse_pos_ = Vec2(mouse.x, mouse.y);
}

Vec2 Player::dir() const {
  return facing_dir_;
}

void Player::attack(Map& map) {
  if (!weapon_) return;
  weapon_->shoot(map, *this, 10); 
  if (weapon_->no_ammo()) weapon_ = std::make_unique<Gun>();
}

void Player::set_weapon(std::unique_ptr<Weapon> weapon) {
  weapon_ = std::move(weapon);
}

bool Player::is_dead() const {
  return hp <= 0;
}

void Player::kill() {
  hp = 0;
}

void Player::on_wall_collision() {
  position = prev_position_;
}

void Player::take_damage(int damage) {
  if (hp > 0) {
    hp -= damage;
  }
}

void Player::slow_down() {
  slow_coeff_ = 0.4f;
}

void Player::take_tile_damage(int damage) {
  if (tile_damage_cooldown_ > 0.0f) return;
  take_damage(damage);
  tile_damage_cooldown_ = 0.5f;
}

void Player::update(float timediff) {
  if (tile_damage_cooldown_ > 0.0f) tile_damage_cooldown_ -= timediff;
  prev_position_ = position;
  move(timediff);
  slow_coeff_ = 1.0f;
  cornrotate = facing_dir_.angle();

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

void Player::serialize(sf::Packet& packet) const {
  packet << network_id << position.x << position.y << cornrotate << static_cast<sf::Int32>(hp) << input_w << input_a << input_s << input_d << mouse_pos_.x << mouse_pos_.y;
}

void Player::deserialize(sf::Packet& packet) {
  sf::Int32 remote_hp;
  packet >> position.x >> position.y >> cornrotate >> remote_hp >> input_w >> input_a >> input_s >> input_d >> mouse_pos_.x >> mouse_pos_.y;
  hp = remote_hp;
  if (hp <= 0) kill();
}

void Player::set_input(bool w, bool a, bool s, bool d) {
  input_w = w; input_a = a; input_s = s; input_d = d;
}
