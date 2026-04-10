#include "../libraries/projectile.hpp"

Projectile::Projectile(Vec2 pos, Vec2 velocity, int damage, int size, sf::IntRect texture_rect = sf::)
    : pos_(pos), velocity_(velocity),
      damage_(damage), size_(size), texture_rect_(texture_rect) {}

void Projectile::update(float timediff) {
    this->pos_ = this->pos_ + Vec2(static_cast<float>(velocity_.x * timediff), static_cast<float>(velocity_.y * timediff));
}

int Projectile::GetDamage() const { return damage_; }
Vec2 Projectile::GetVelocity() const { return velocity_; }

bool Projectile::isDead() const {
    return dead;
}
void Projectile::kill() {
    dead = true;
}
void Projectile::draw(sf::RenderWindow& window) {

}
