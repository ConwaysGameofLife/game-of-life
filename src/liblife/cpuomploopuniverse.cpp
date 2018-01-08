#include "universe.h"

#ifdef _OPENMP

class CpuOmpLoopUniverse : public AbstractCpuUniverse {
public:
  CpuOmpLoopUniverse(int width, int height)
      : AbstractCpuUniverse(width, height) {}

  std::string name() const { return "CPU Loop with OpenMP"; }

protected:
  void next(Vec2d &src, Vec2d &dst) const final { nextLoopOmp(src, dst); }
};

#endif // _OPENMP
