#include "collision.hpp"
#include "map/map.hpp"
#include <iostream>

bool Collision::checkAABB(const Vec2& posA, const int sizeA,
                          const Vec2& posB, const int sizeB) {
    double dist = sqrt((posA.x - posB.x) * (posA.x - posB.x) + (posA.y - posB.y) * (posA.y - posB.y));
    return (dist - ((sizeA + sizeB) / 2.0) > 0); // проверка ведется как будто это 2 круга. Возвраща. что обьекты не пересекаются
}

bool Collision::entity_tile(const Player& e, const Map& map) {
    // пиксельные границы карты — проверяем все 4 угла хитбокса
  if (e.position.x < 0 || e.position.y < 0 || ((e.position.x + e.size) / 32) >= map.width_ ||
    ((e.position.y + e.size) / 32 ) >= map.height_) { return true; }
    
  //std::cout << e.position.x << " " << e.position.y << " " << e.size <<  std::endl;
  int x0 = static_cast<int>(e.position.x) / 32;
  int y0 = static_cast<int>(e.position.y) / 32;
  int x1 = static_cast<int>(e.position.x + e.size - 1) / 32;
  int y1 = static_cast<int>(e.position.y + e.size - 1) / 32;
  return map.tiles_[x0][y0].isWall() || map.tiles_[x1][y0].isWall() ||
          map.tiles_[x0][y1].isWall() || map.tiles_[x1][y1].isWall();
}

bool Collision::projectile_tile(const Projectile& proj, const Map& map) {
  if (proj.position.x < 0 || proj.position.y < 0 || proj.position.x + proj.size > map.width_ * 32 ||
      proj.position.y + proj.size > map.height_ * 32) { return true; }
  int x0 = static_cast<int>(proj.position.x) / 32;
  int y0 = static_cast<int>(proj.position.y) / 32;
  int x1 = static_cast<int>(proj.position.x + proj.size - 1) / 32;
  int y1 = static_cast<int>(proj.position.y + proj.size - 1) / 32;
  return map.tiles_[x0][y0].isWall() || map.tiles_[x1][y0].isWall() ||
          map.tiles_[x0][y1].isWall() || map.tiles_[x1][y1].isWall();
}

void Collision::resolve(Map& map) {
    for (auto& p : map.projectiles_) {
        if (projectile_tile(*p, map)) {
            p->kill();
            continue;
        }
    }
    int cnt = 0;
    for (auto& p : map.projectiles_) {
        if (p->isDead()) continue;
        if (p->lifetime() < 0.5) continue;
        for (auto& e : map.entities_) {
            if (e->isDead()) continue;
            if (!checkAABB(p->position, p->size, e->position, e->size)) {
                e->take_damage(p->damage);
                p->kill();
                std::cout << "ТОЧНО В ЦЕЛЬ" << cnt << '\n';  // при проверке игра считает игрока, выпустившего пули, 
                                                             //за того кто их потом в лоб получает                        
                cnt++;
                break;
            }
        }
    }
    //std::cout << "ALIVE\n";
    for (auto& e : map.entities_) {
        if (entity_tile(*e, map)) {
            e->on_wall_collision();
        }
    }

}