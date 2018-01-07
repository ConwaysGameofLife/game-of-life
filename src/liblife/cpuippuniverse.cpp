#include "universe.h"

class CpuIppUniverse : public AbstractCpuUniverse {
public:
  CpuIppUniverse(int width, int height) : AbstractCpuUniverse(width, height) {}

  std::string name() const { return "CPU Single-Threaded IPP"; }

protected:
  void next(Vec2d &src, Vec2d &dst) const final {
    nextIpp(src, dst);
  }
};
