#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "iuniverse.h"
#include "gameconfig.h"
#include "random.h"

class BigBang {
public:
  using factory_t = std::function<std::unique_ptr<IUniverse>(int, int)>;

  static std::unique_ptr<IUniverse> create(const std::string &name, int width,
                                           int height) {
    auto u = factories_[name](width, height);
    for (int i = 0; i < width * height * GameConfig::BORN_RATE; ++i) {
      int x = Random::next<int>(width - 1);
      int y = Random::next<int>(height - 1);
      u->add(x, y);
    }
    return u;
  }

  template <typename T> static void add() {
    const auto &name = T::name();
    factories_[name] = [](int w, int h) {
      return std::unique_ptr<IUniverse>(new T(w, h));
    };
  }

  static void init();

private:
  static std::unordered_map<std::string, factory_t> factories_;
};
