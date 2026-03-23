#include "player.hpp"

Player::Player(float cord_x, float cord_y, int hp, float speed, Weapon weapon)
    : Character(cord_x, cord_y, hp, speed, weapon) {}

void Player::move(float timediff) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) cord_y_ -= speed_ * timediff;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) cord_y_ += speed_ * timediff;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) cord_x_ -= speed_ * timediff;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) cord_x_ += speed_ * timediff;
}

void Player::attack() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        weapon_.shoot(cord_x_, cord_y_);
    }
}
