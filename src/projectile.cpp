#include "../libraries/projectile.hpp"

Projectile::Projectile(Vec2 pos, Vec2 velocity, int damage, int size)
    : Entity(pos, damage, size), velocity_(velocity), damage_(damage), size_(size) {}

Projectile::~Projectile() = default;

void Projectile::update(float timediff) {
  position = position + Vec2(velocity_.x * timediff, velocity_.y * timediff);
}

int Projectile::GetDamage() const { return damage_; }
Vec2 Projectile::GetVelocity() const { return velocity_; }

bool Projectile::isDead() const {
  return dead;
}
void Projectile::kill() {
  dead = true;
}

void Projectile::on_wall_collision() {
   dead = true;
}

void Projectile::take_damage(int dmg) {
    //Временная заглушка
}
