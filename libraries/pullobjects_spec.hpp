#pragma once
#include <queue>
#include "pullobjects.hpp"
#include "projectile.hpp"
#include "player.hpp"

template<>
class PullObjects<Projectile> {
  private:
  std::queue<std::pair<Projectile, float>> objects_;
  float map_size_ = 600.0f;
 public:
  void add(Projectile obj, float current_time) {
    objects_.push({obj, current_time});
  }
  void cleanup(float current_time) {
    while (!objects_.empty()) {
      if (objects_.front().second <= current_time - (map_size_ / objects_.front().first.GetVelocity())) {
        objects_.pop();
      }
      else break;
    }
  }
};  // ok

template<>
class PullObjects<Player> {
 private:
  std::vector<Player> players_;
 public:
  void add(Player player) {
    if (players_.size() < 4) {
      players_.push_back(player);
    }
  }
  void remove(int index) {
    players_.erase(players_.begin() + index);
  }
};
