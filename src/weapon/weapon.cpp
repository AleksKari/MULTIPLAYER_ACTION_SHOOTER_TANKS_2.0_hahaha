#include "weapon.hpp"
#include "../map/map.hpp"


void Weapon::shoot(Map& map, Entity& owner) {
    if (cooldown_timer > 0) {
        return;
    }
    Singshot 
    fireMode->shoot(map, owner, damage)
    cooldown_timer = cooldown;
}
