#pragma once
#include <Entity.hpp>
#include <SFML/Network.hpp>
#include <memory>
#include <string>
#include <weapon/Weapon.hpp>

class Weapon;
class Map;

const int DAMAGE_FROM_SHOOT = 10;

class Player : public Entity {
 private:
  Vec2 mouse_pos_;
  Vec2 prev_position_;
  Vec2 facing_dir_{1.0, 0.0};
  float speed_;
  float slow_coeff_ = 1.0F;
  float tile_damage_cooldown_ = 0.0F;
  int size_ = TILESIZE;
  std::unique_ptr<Weapon> weapon_;
  sf::Texture texture_;
  sf::Sprite sprite_;

 public:
  int max_hp;
  int heatpoint;
  sf::Uint32 network_id = 0;
  bool is_local = false;
  bool input_w = false, input_a = false, input_s = false, input_d = false;

  sf::Sprite& GetSprite() { return sprite_; }
  const sf::Sprite& GetSprite() const { return sprite_; }

  Player(Vec2 pos, int heatpoint, float speed, std::unique_ptr<Weapon> weapon);
  Vec2 Dir() const;
  void Move(float timediff);
  void Attack(Map& map);
  void Kill();
  void OnWallCollision();
  void TakeDamage(int damage);
  void Update(float timediff);
  void SetMouse(sf::Vector2i mouse);
  void LoadSkin(const std::string& path);
  void SetWeapon(std::unique_ptr<Weapon> weapon);
  bool IsDead() const;
  void SlowDown();
  void TakeTileDamage(int damage);

  void Serialize(sf::Packet& packet) const;
  void Deserialize(sf::Packet& packet);
  void SetInput(bool button_w, bool button_a, bool button_s, bool button_d);
};
