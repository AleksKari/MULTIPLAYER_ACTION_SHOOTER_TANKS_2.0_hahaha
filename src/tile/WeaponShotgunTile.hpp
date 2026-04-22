#include "WeaponTile.hpp"

class WeaponShotgunTile : public WeaponTile {
  public:
    sf::Texture texture_;
    WeaponShotgunTile() {
        texture_.loadFromFile("textures/map.png"); //должна быть своя текстура
    }
};