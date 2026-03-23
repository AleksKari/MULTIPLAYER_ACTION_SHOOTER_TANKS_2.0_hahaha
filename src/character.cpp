#include "character.hpp"
Character::Character(float cord_x, float cord_y, int hp, float speed, Weapon weapon)
    : Entity(cord_x, cord_y, hp, speed), weapon_(weapon) {}
void Character::update(float timediff) {
  move(timediff);
}
void Character::draw(sf::RenderWindow& window) const {
  sf::RectangleShape shape({40.f, 60.f});
  shape.setPosition(cord_x_, cord_y_);
  window.draw(shape);
}

Weapon Character::GetWeapon() const { return weapon_; }
void Character::attack() {/*атака протвника*/}