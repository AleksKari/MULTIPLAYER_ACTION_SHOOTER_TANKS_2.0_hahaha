#include <projectile.hpp>

Projectile::Projectile(Vec2 pos, Vec2 velocity, int damage, int size, int hp)
    : Entity(pos, damage, size), velocity_(velocity), damage_(damage), 
    size_(size), hp_(hp), prev_position_(pos), created_(std::chrono::steady_clock::now()) {}

Projectile::~Projectile() = default;

void Projectile::update(float timediff) {
  prev_position_ = position;
  position = position + Vec2(velocity_.x * timediff, velocity_.y * timediff);
}

int Projectile::get_damage() const { return damage_; }
Vec2 Projectile::get_velocity() const { return velocity_; }

bool Projectile::is_dead() const {
  return hp_ <= 0;
}
void Projectile::kill() {
  hp_ = 0;
}

void Projectile::on_wall_collision() {
  hp_ = 0;
}

double Projectile::lifetime() const {
  if (hp_ <= 0) return 0;
  return std::chrono::duration<double>(std::chrono::steady_clock::now() - created_).count();
}

void Projectile::take_damage(int dmg) {
  hp_ -= dmg;
}

bool Projectile::can_ricochet() const { return hp_ > 1; }
