#include "../libraries/player.hpp"
#include "map/map.hpp"
#include "weapon/weapon.hpp"

Player::Player(Vec2 pos, int hp, float speed, std::unique_ptr<Weapon> weapon)
    : position_(pos), hp_(hp), speed_(speed), weapon_(std::move(weapon)) {}

void Player::move(float timediff) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) position_.y -= speed_ * timediff;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) position_.y += speed_ * timediff;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) position_.x -= speed_ * timediff;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) position_.x += speed_ * timediff;
}

Vec2 Player::dir() const {
  sf::Vector2i mouse = sf::Mouse::getPosition();
  return Vec2(mouse.x - position_.x, mouse.y - position_.y);
}

void Player::attack(Map& map) {
  sf::Vector2i mouse = sf::Mouse::getPosition();
  Vec2 direct(mouse.x - position_.x, mouse.y - position_.y);  
  weapon_->shoot(map, *this, 10); // пока damage будет 10 
}

bool Player::isDead() const {
    return hp_ <= 0;
}
//вроде так но если что снос alive
void Player::kill() {
  if (isDead()){
    alive = false;
  }
}
//не даем застрять откатывает координаты
void Player::on_wall_collision() {
    position_.x -= position_.x * 0.1f;
    position_.y -= position_.y * 0.1f;
}

void Player::take_damage(int damage) {
  if(hp_ > 0) {
    hp_ -= damage;
  }
}
//есть в энтити убрать нельзя но по сути копипаст
void Player::update(float timediff) {
    move(timediff);
}
//подключить текстуру когда будет
void Player::draw(sf::RenderWindow& window) const {
    sf::RectangleShape shape(sf::Vector2f(size_, size_));
    shape.setPosition(position_.x, position_.y);
    shape.setTexture(&texture_);
    window.draw(shape);
}