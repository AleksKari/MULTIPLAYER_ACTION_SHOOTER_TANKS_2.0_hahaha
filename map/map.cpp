#include "../libraries/entity.hpp"
#include "../libraries/projectile.hpp"

class Map {
  public:
    Map(int width, int height);

    void update(double dt);
    void spawn_entity(Entity* e);
    void spawn_projectile(Projectile* pr);
  private:
    int width;
    int height;
    //массив с неигровыми обьектами(стены maybe)
    std::vector<Entity> entities;
    std::vector<Projectile> projectiles;
};