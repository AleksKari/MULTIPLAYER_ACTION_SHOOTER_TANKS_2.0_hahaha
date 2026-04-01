#include "../libraries/entity.hpp"
#include "../libraries/projectile.hpp"

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
        //Collision::resolve(*this);
    }
    void spawn_entity(Entity* e);
    void spawn_projectile(Projectile* pr);
  private:
    int width;
    int height;
    //массив с неигровыми обьектами(стены maybe)
    std::vector<Entity*> entities;
    std::vector<Projectile*> projectiles;
};