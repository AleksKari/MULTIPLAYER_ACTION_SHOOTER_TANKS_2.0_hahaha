#include "tile.hpp"

Tile::Tile(Type type, const sf::IntRect& rect) : type(type), texture_rect(rect) {}
bool Tile::isBreakable() const {
    return type == Type::Wall;
}
bool Tile::isDamage() const {
    return type == Type::Damage;
}
bool Tile::isEmpty() const {
    return type == Type::Empty;
}
bool Tile::isSlow() const {
    return type == Type::Slow;
}
bool Tile::isWall() const {
    return type == Type::Wall;
}