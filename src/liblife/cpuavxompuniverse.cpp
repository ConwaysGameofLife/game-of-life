#include "universe.h"

#ifdef _OPENMP

class CpuAvxOmpUniverse : public AbstractCpuUniverse {
public:
  CpuAvxOmpUniverse(int width, int height)
      : AbstractCpuUniverse(width, height) {}

  std::string name() const { return "CPU AVX OpenMP"; }

protected:
  void next(Vec2d &src, Vec2d &dst) const final {
    nextAvxOmp(src, dst);
  }
};

#endif // _OPENMP
