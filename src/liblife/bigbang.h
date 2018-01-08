#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include "iuniverse.h"

class BigBang {
public:
  using factory_t = std::function<IUniverse *(int, int)>;

  static std::unique_ptr<IUniverse> create(const std::string &name, int width,
                                           int height) {
    return std::unique_ptr<IUniverse>(factories_[name](width, height));
  }
  static void add(const std::string &name, const factory_t &factory) {
    factories_[name] = factory;
  }

private:
  static std::unordered_map<std::string, factory_t> factories_;
};

struct BigBangRegister {
  BigBangRegister(const std::string &name, const BigBang::factory_t &factory) {
    BigBang::add(name, factory);
  }
};
