#include "../libraries/projectile.hpp"

Projectile::Projectile(Vec2 pos, Vec2 velocity, int damage, int size)
    : pos_(pos), velocity_(velocity),
      damage_(damage), size_(size) {}

void Projectile::update(float timediff) {
    pos = pos + Vec2(static_cast<float>(velocity_.x * timediff), static_cast<float>(velocity_.y * timediff));
}

int Projectile::GetDamage() const { return damage_; }
Vec2 Projectile::GetVelocity() const { return velocity_; }

bool Projectile::isDead() const {
    return dead;
}
void Projectile::kill() {
    dead = true;
}
