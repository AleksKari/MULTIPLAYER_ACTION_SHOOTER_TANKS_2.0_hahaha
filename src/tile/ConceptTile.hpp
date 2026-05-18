#pragma once
#include <concepts>

#include <tile/Tile.hpp>

template <typename T>
concept is_tile = std::derived_from<T, Tile>;
