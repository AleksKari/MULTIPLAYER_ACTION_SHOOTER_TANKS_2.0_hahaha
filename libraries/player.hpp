#pragma once
#include "entity.hpp"
#include "../src/weapon/weapon.hpp"

class Player : public Entity {
  public:
    Player(Vec2 pos, int hp, float speed, Weapon weapon);
    void move(float timediff);
    void attack();
    bool isDead() const;
    void kill();
    void on_wall_collision();
    void take_damage(int damage);
    void update(float timediff);
    void draw(sf::RenderWindow& window) const;
  private:
    Vec2 position_;
    int hp_;
    float speed_;
    Weapon weapon_;
    bool alive = true;
    int size_ = 32;
    sf::Texture texture_;//по сути рендер рисует но тут метод дров надо текстурку подключить
};
