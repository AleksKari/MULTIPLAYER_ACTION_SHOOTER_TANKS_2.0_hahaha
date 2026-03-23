#include "entity.hpp"

Entity::Entity(float cord_x,float cord_y,int hp,float speed) : cord_x_(cord_x), cord_y_(cord_y),
                                                                hp_(hp), speed_(speed) {}

void Entity::takeDamage(int numb) {
  hp_ -= numb;
  if (hp_ < 0) hp_ = 0;
}

float Entity::GetX() const { return cord_x_; }
float Entity::GetY() const { return cord_y_; }
int Entity::GetHp() const { return hp_; }
float Entity::GetSpeed() const { return speed_; }
bool Entity::IsAlive() const {return hp_ > 0; }