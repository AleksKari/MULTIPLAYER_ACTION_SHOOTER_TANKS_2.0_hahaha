#pragma once
#include <vector>

template<typename T>
class PullObjects {
 private:
  std::vector<T> objects_;
 public:
  void add(T obj) {
    objects_.push_back(obj);
  }
  void remove(int index) {
    objects_.erase(objects_.begin() + index);
  }
};
