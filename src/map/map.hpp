#include "../libraries/entity.hpp"
#include "../libraries/projectile.hpp"
#include "../tile/tile.hpp"
#include <memory>

class Map {
  public:
    Map(int width, int height) : width(width), height(height) {}

    void update(double dt){
        for (auto& e : entities) {
            e->update(dt);
        }
        for (auto& pr : projectiles) {
            pr->update(dt);
        }
        Collision::resolve(*this);
    }
    void spawn_entity(Entity* e);
    void spawn_projectile(Projectile* pr);
    bool isWall(int pos_x, int pos_y) const; // проверяет что текущий ТАЙЛ это стена
    bool isEmpty(int pos_x, int pos_y) const;//тайл пуст
    bool isSlow(int pos_x, int pos_y) const;//тайл замедляет
    bool isDamage(int pos_x, int pos_y) const;//тайл наносит урон
    bool isBreakable(int pos_x, int pos_y) const;//тайл разрушаем
    bool isBound(int pos_x, int pos_y) const;//тайл в пределах карты
    friend class Collision;
  private:
    int width;
    int height;
    int tile_size;
    std::vector<std::vector<Tile>> tiles;
    std::vector<std::unique_ptr<Entity>> entities;
    std::vector<std::unique_ptr<Projectile>> projectiles;
};