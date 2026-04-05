#include "projectile.hpp"

Projectile::Projectile(float x, float y, int damage, Vec2 velocity)
    : Entity(x, y, 1, static_cast<float>(velocity.length())),
      damage_(damage), velocity_(velocity) {}

void Projectile::update(float timediff) {
    cord_x_ += static_cast<float>(velocity_.x * timediff);
    cord_y_ += static_cast<float>(velocity_.y * timediff);
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
