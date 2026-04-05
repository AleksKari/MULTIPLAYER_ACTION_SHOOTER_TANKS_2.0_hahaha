#include "../libraries/player.hpp"

Player::Player(Vec2 pos, int hp, float speed, Weapon weapon, int size)
    : pos(pos), hp(hp) {}

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




