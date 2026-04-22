#include "WeaponTile.hpp"

class WeaponGunTile : public WeaponTile {
  public:
    sf::Texture texture_;
    WeaponGunTile() {
        texture_.loadFromFile("textures/map.png"); //должна быть своя текстура
    }
};