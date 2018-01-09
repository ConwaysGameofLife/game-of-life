#include "bigbang.h"

#include "cpuavxuniverse.h"
#include "gameconfig.h"
#include "random.h"

std::unordered_map<std::string, BigBang::factory_t> BigBang::factories_;

std::unique_ptr<IUniverse> BigBang::create(const std::string &name, int width,
                                           int height) {
  auto u = factories_[name](width, height);
  for (int i = 0; i < width * height * GameConfig::BORN_RATE; ++i) {
    int x = Random::next<int>(width - 1);
    int y = Random::next<int>(height - 1);
    u->add(x, y);
  }
  return u;
}

void BigBang::init() { add<CpuAvxUniverse>(); }
