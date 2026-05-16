#pragma once
#include "Tile.hpp"
#include <concepts>

template<typename T>
concept is_tile = std::derived_from<T, Tile>;
