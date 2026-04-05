#include "../libraries/projectile.hpp"

Projectile::Projectile(Vec2 pos, Vec2 velocity, int damage, int size)
    : pos(pos), velocity_(velocity),
      damage_(damage), size(size) {}

void Projectile::update(float timediff) {
    pos += Vec2(static_cast<float>(velocity_.x * timediff), static_cast<float>(velocity_.y * timediff));
}

void Projectile::draw(sf::RenderWindow& window) const {
    sf::CircleShape shape(4.f);
    shape.setPosition(cord_x_, cord_y_);
    shape.setFillColor(sf::Color::Yellow);
    window.draw(shape);
}

int Projectile::GetDamage() const { return damage_; }
Vec2 Projectile::GetVelocity() const { return velocity_; }

bool Projectile::isDead() const {
    return dead;
}
void Projectile::kill() {
    dead = true;
}
