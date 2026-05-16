#pragma once
#include <entity.hpp>
#include <concepts>

template<typename T>
concept is_entity = std::derived_from<T, Entity>;
