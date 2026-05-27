#pragma once

#include <SFML/Graphics.hpp>

class Renderer;
class Vec2;
class Player;
class Map;

class Tile {
 public:
  virtual ~Tile() = default;
  virtual bool IsEmpty() { return false; }
  virtual bool IsWall() { return false; }
  virtual bool IsBreakable() { return false; }
  virtual bool IsSlow() { return false; }
  virtual bool IsDamage() { return false; }
  virtual bool IsWeapon() { return false; }
  virtual void Draw(Renderer& renderer, Vec2 pos) = 0;
  virtual void Interact(Player& player, Map& map, const Vec2& pos) = 0;
};
