#include <Projectile.hpp>

Projectile::Projectile(Vec2 pos, Vec2 velocity, int damage, int size,
                       int heatpoint, const Player* source)
    : Entity(pos, damage, size),
      velocity_(velocity),
      damage_(damage),
      size_(size),
      hp_(heatpoint),
      prev_position_(pos),
      created_(std::chrono::steady_clock::now()),
      owner(source) {}

Projectile::~Projectile() = default;

void Projectile::Update(float timediff) {
  prev_position_ = position;
  position =
      position + Vec2(velocity_.cord_x * timediff, velocity_.cord_y * timediff);
}

int Projectile::GetDamage() const { return damage_; }
int Projectile::GetHp() const { return hp_; }
Vec2 Projectile::GetVelocity() const { return velocity_; }

bool Projectile::IsDead() const { return hp_ <= 0; }
void Projectile::Kill() { hp_ = 0; }

void Projectile::OnWallCollision() { hp_ = 0; }

double Projectile::Lifetime() const {
  if (hp_ <= 0) {
    return 0;
  }
  return std::chrono::duration<double>(std::chrono::steady_clock::now() -
                                       created_)
      .count();
}

void Projectile::TakeDamage(int damage) { hp_ -= damage; }

bool Projectile::CanRicochet() const { return hp_ > 1; }

bool Projectile::CrossedTileX() const {
  return (static_cast<int>(position.cord_x) / TILESIZE !=
          static_cast<int>(prev_position_.cord_x) / TILESIZE) ||
         (static_cast<int>(position.cord_x + size) / TILESIZE !=
          static_cast<int>(prev_position_.cord_x + size) / TILESIZE);
}

bool Projectile::CrossedTileY() const {
  return (static_cast<int>(position.cord_y) / TILESIZE !=
          static_cast<int>(prev_position_.cord_y) / TILESIZE) ||
         (static_cast<int>(position.cord_y + size) / TILESIZE !=
          static_cast<int>(prev_position_.cord_y + size) / TILESIZE);
}

void Projectile::Reflect(bool by_x, bool by_y) {
  if (by_x) {
    velocity_.cord_x = -velocity_.cord_x;
  }
  if (by_y) {
    velocity_.cord_y = -velocity_.cord_y;
  }
  position = prev_position_;
}
