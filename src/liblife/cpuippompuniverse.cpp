#include "universe.h"

#ifdef _OPENMP

class CpuIppOmpUniverse : public AbstractCpuUniverse {
public:
  CpuIppOmpUniverse(int width, int height)
      : AbstractCpuUniverse(width, height) {}

  std::string name() const { return "CPU IPP with OpenMP"; }

protected:
  void next(Vec2d &src, Vec2d &dst) const final {
    nextIppOmp(src, dst);
  }
};

#endif // _OPENMP
