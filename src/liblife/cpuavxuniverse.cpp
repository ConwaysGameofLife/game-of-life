#include "universe.h"

#include "bigbang.h"

#include <algorithm>
#include <cstring>

class CpuAvxUniverse : public AbstractCpuUniverse {
public:
  CpuAvxUniverse(int width, int height) : AbstractCpuUniverse(width, height) {
    sum = aligned_malloc<uint8_t>(width - 2, 32);
  }
  ~CpuAvxUniverse() final { aligned_free(sum); }

  std::string name() const { return "CPU AVX"; }

  void next(int width, int height, uint8_t *src, int srcStride, int srcInc,
            uint8_t *dst, int dstStride, int dstInc) const;

protected:
  void next(Vec2d &src, Vec2d &dst) const final;

private:
  uint8_t *sum;
};

void CpuAvxUniverse::next(Vec2d &src, Vec2d &dst) const {
  next(src.width(), src.height(), src.data(), src.getRowBytes(),
       src.getIncrement(), dst.data(), dst.getRowBytes(), dst.getIncrement());
}

void CpuAvxUniverse::next(int width, int height, uint8_t *src, int srcStride,
                          int srcInc, uint8_t *dst, int dstStride,
                          int dstInc) const {
  for (int r = 0; r < height; ++r) {
    *(src + r * srcStride) = 0x00;
    *(src + r * srcStride + (width - 1) * srcInc) = 0x00;
  }
  std::fill(src, src + (width - 1) * srcInc, 0x00);
  std::fill(src + (height - 1) * srcStride,
            src + (height - 1) * srcStride + (width - 1) * srcInc, 0x00);

  for (int r = 0; r < height; ++r)
    ui8vandcu(width, src + srcStride * r, src + srcStride * r, 1);

  const int x[] = {
      -1, 0, 1, -1, 1, -1, 0, 1,
  };
  const int y[] = {
      -1, -1, -1, 0, 0, 1, 1, 1,
  };

  for (int r = 1; r < height - 1; ++r) {
    uint8_t *row = src + srcStride * r + srcInc;
    uint8_t *dst_row = dst + dstStride * r + dstInc;
    memcpy(sum, row + y[0] * srcStride + x[0] * srcInc, width - 2);
    for (int i = 1; i < 8; ++i)
      ui8vaddu(width - 2, sum, row + y[i] * srcStride + x[i] * srcInc, sum);

    memcpy(dst_row, sum, width - 2);

    ui8vcmpcu(width - 2, sum, sum, 2, ivEq); // 2 => 255
    ui8vandu(width - 2, sum, row, sum);      // 255 & 1
    ui8vcmpcu(width - 2, sum, sum, 1, ivEq); // 1 => 255

    ui8vcmpcu(width - 2, dst_row, dst_row, 3, ivEq); // 3 => 255

    ui8voru(width - 2, sum, dst_row, dst_row);
  }
}

BigBangRegister a("CpuAvxUniverse",
                  [](int w, int h) { return new CpuAvxUniverse(w, h); });
