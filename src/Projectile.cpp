#include <Projectile.hpp>

Projectile::Projectile(Vec2 pos, Vec2 velocity, int damage, int size, int hp,
                       const Player* source)
    : Entity(pos, damage, size),
      velocity_(velocity),
      damage_(damage),
      size_(size),
      hp_(hp),
      prev_position_(pos),
      created_(std::chrono::steady_clock::now()),
      owner(source) {}

Projectile::~Projectile() = default;

void Projectile::update(float timediff) {
  prev_position_ = position;
  position = position + Vec2(velocity_.cord_x * timediff, velocity_.cord_y * timediff);
}

int Projectile::get_damage() const { return damage_; }
int Projectile::get_hp() const { return hp_; }
Vec2 Projectile::get_velocity() const { return velocity_; }

bool Projectile::is_dead() const { return hp_ <= 0; }
void Projectile::kill() { hp_ = 0; }

void Projectile::on_wall_collision() { hp_ = 0; }

double Projectile::lifetime() const {
  if (hp_ <= 0) return 0;
  return std::chrono::duration<double>(std::chrono::steady_clock::now() -
                                       created_)
      .count();
}

void Projectile::take_damage(int dmg) { hp_ -= dmg; }

bool Projectile::can_ricochet() const { return hp_ > 1; }

bool Projectile::crossed_tile_x() const {
  return (static_cast<int>(position.cord_x) / TILESIZE !=
          static_cast<int>(prev_position_.cord_x) / TILESIZE) ||
         (static_cast<int>(position.cord_x + size) / TILESIZE !=
          static_cast<int>(prev_position_.cord_x + size) / TILESIZE);
}

bool Projectile::crossed_tile_y() const {
  return (static_cast<int>(position.cord_y) / TILESIZE !=
          static_cast<int>(prev_position_.cord_y) / TILESIZE) ||
         (static_cast<int>(position.cord_y + size) / TILESIZE !=
          static_cast<int>(prev_position_.cord_y + size) / TILESIZE);
}

void Projectile::reflect(bool coord_x, bool coord_y) {
  if (coord_x) velocity_.cord_x = -velocity_.cord_x;
  if (coord_y) velocity_.cord_y = -velocity_.cord_y;
  position = prev_position_;
}
