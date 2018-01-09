#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include "iuniverse.h"

class BigBang {
public:
  using factory_t = std::function<std::unique_ptr<IUniverse>(int, int)>;

  static void init();
  static std::unique_ptr<IUniverse> create(const std::string &name, int width,
                                           int height);

  template <typename T> static void add() {
    const auto &name = T::name();
    factories_[name] = [](int w, int h) {
      return std::unique_ptr<IUniverse>(new T(w, h));
    };
  }

private:
  static std::unordered_map<std::string, factory_t> factories_;
};
