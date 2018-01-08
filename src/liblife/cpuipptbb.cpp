#include "universe.h"

class CpuIppTbbUniverse : public AbstractCpuUniverse {
public:
  CpuIppTbbUniverse(int width, int height)
      : AbstractCpuUniverse(width, height) {}

  std::string name() const { return "CPU IPP with TBB"; }

protected:
  void next(Vec2d &src, Vec2d &dst) const final {
    nextIppTbb(src, dst);
  }
};
