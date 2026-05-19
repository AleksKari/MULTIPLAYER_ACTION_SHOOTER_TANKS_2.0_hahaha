#include <Player.hpp>
#include <iostream>
#include <map/Map.hpp>
#include <weapon/Gun.hpp>
#include <weapon/Weapon.hpp>

const float MAX_COORD_TO_CHANGE_DIR = 900.F;
const float SLOW_COEFF = 0.4F;
const float TILE_DMG_COOLDDOWN = 0.5F;

Player::Player(Vec2 pos, int heatpoint, float speed,
               std::unique_ptr<Weapon> weapon)
    : Entity(pos, 0, TILESIZE),
      heatpoint(heatpoint),
      max_hp(heatpoint),
      speed_(speed),
      weapon_(std::move(weapon)) {
  if (position.cord_x > MAX_COORD_TO_CHANGE_DIR) {
    facing_dir_ = Vec2(-1.0, 0.0);
  }
}

void Player::move(float timediff) {
  Vec2 input_dir(static_cast<int>(input_d) - static_cast<int>(input_a),
                 static_cast<int>(input_s) - static_cast<int>(input_w));
  const Vec2 normalized = input_dir.normalized();
  if (normalized.length() > 0.0) {
    facing_dir_ = normalized;
  }
  position = position + normalized * speed_ * slow_coeff_ * timediff;
}

void Player::set_mouse(sf::Vector2i mouse) {
  mouse_pos_ = Vec2(mouse.x, mouse.y);
}

Vec2 Player::dir() const { return facing_dir_; }

void Player::attack(Map& map) {
  if (!weapon_) {
    return;
  }
  weapon_->shoot(map, *this, DAMAGE_FROM_SHOOT);
  if (weapon_->no_ammo()) {
    weapon_ = std::make_unique<Gun>();
  }
}

void Player::set_weapon(std::unique_ptr<Weapon> weapon) {
  weapon_ = std::move(weapon);
}

bool Player::is_dead() const { return heatpoint <= 0; }

void Player::kill() { heatpoint = 0; }

void Player::on_wall_collision() { position = prev_position_; }

void Player::take_damage(int damage) {
  if (heatpoint > 0) {
    heatpoint -= damage;
  }
}

void Player::slow_down() { slow_coeff_ = SLOW_COEFF; }

void Player::take_tile_damage(int damage) {
  if (tile_damage_cooldown_ > 0.0F) {
    return;
  }
  take_damage(damage);
  tile_damage_cooldown_ = TILE_DMG_COOLDDOWN;
}

void Player::update(float timediff) {
  if (tile_damage_cooldown_ > 0.0F) {
    tile_damage_cooldown_ -= timediff;
  }
  prev_position_ = position;
  move(timediff);
  slow_coeff_ = 1.0F;
  cornrotate = facing_dir_.angle();

  sprite_.setPosition(position.cord_x + (size_ / NUMBER_TO_DIV),
                      position.cord_y + (size_ / NUMBER_TO_DIV));
  sprite_.setRotation(cornrotate);
}

void Player::loadSkin(const std::string& path) {
  if (!texture_.loadFromFile(path)) {
    std::cerr << "Error loading skin: " << path << "\n";
    return;
  }
  sprite_.setTexture(texture_);
  sf::FloatRect bounds = sprite_.getLocalBounds();

  sprite_.setOrigin(bounds.width / NUMBER_TO_DIV,
                    bounds.height / NUMBER_TO_DIV);
  sprite_.setScale(static_cast<float>(size_) / bounds.width,
                   static_cast<float>(size_) / bounds.height);
}

void Player::serialize(sf::Packet& packet) const {
  packet << network_id << position.cord_x << position.cord_y << cornrotate
         << static_cast<sf::Int32>(heatpoint) << input_w << input_a << input_s
         << input_d << mouse_pos_.cord_x << mouse_pos_.cord_y;
}

void Player::deserialize(sf::Packet& packet) {
  sf::Int32 remote_hp;
  float remote_x;
  float remote_y;
  float rrot;

  packet >> remote_x >> remote_y >> rrot >> remote_hp >> input_w >> input_a >>
      input_s >> input_d >> mouse_pos_.cord_x >> mouse_pos_.cord_y;

  heatpoint = remote_hp;
  if (heatpoint <= 0) {
    kill();
  }

  if (!is_local) {
    position.cord_x = remote_x;
    position.cord_y = remote_y;
    cornrotate = rrot;
  }

  sprite_.setPosition(position.cord_x + (size_ / NUMBER_TO_DIV),
                      position.cord_y + (size_ / NUMBER_TO_DIV));
  sprite_.setRotation(cornrotate);
}

void Player::set_input(bool button_w, bool button_a, bool button_s,
                       bool button_d) {
  input_w = button_w;
  input_a = button_a;
  input_s = button_s;
  input_d = button_d;

  Vec2 input_dir(static_cast<int>(input_d) - static_cast<int>(input_a),
                 static_cast<int>(input_s) - static_cast<int>(input_w));
  const Vec2 normalized = input_dir.normalized();
  if (normalized.length() > 0.0) {
    facing_dir_ = normalized;
    cornrotate = facing_dir_.angle();
  }
}