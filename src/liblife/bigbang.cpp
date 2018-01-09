#include "bigbang.h"

#include "cpuavxuniverse.h"

std::unordered_map<std::string, BigBang::factory_t> BigBang::factories_;

void BigBang::init() {
  add<CpuAvxUniverse>();
}
