#pragma once
#include "entity.hpp"
#include "../src/weapon/weapon.hpp"

class Weapon;

class Player : public Entity {
  public:
    Player(Vec2 pos, int hp, float speed, std::unique_ptr<Weapon> weapon);
    void move(float timediff);
    Vec2 dir() const;
    void attack(Map& map);
    bool isDead() const;
    void kill();
    void on_wall_collision();
    void take_damage(int damage);
    void update(float timediff);
    void set_mouse(sf::Vector2i mouse);
  private:
    int hp_;
    Vec2 mouse_pos_;
    float speed_;
    std::unique_ptr<Weapon> weapon_;
    bool alive = true;
    int size_ = 32;
    sf::Texture texture_;//по сути рендер рисует но тут метод дров надо текстурку подключить
};
