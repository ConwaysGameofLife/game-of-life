#include "universe.h"

#include <algorithm>

class CPUSingleThreadedLoopUniverse : public AbstractCpuUniverse {
public:
  CPUSingleThreadedLoopUniverse(int width, int height)
      : AbstractCpuUniverse(width, height) {}

  std::string name() const { return "CPU Single-Threaded Loop"; }

protected:
  void next(Vec2d &src, Vec2d &dst) const final {
    assert(src.width() == dst.width());
    assert(src.height() == dst.height());

    auto pSrcData = src.data();
    auto cbSrcRow = src.getRowBytes();
    auto cbSrcInc = src.getIncrement();

    auto pDstData = dst.data();
    auto cbDestRow = dst.getRowBytes();
    auto cbDestInc = dst.getIncrement();

    auto height = src.height();
    auto width = src.width();

    for (int r = 0; r < height; ++r) {
      *(pSrcData + r * cbSrcRow) = 0x00;
      *(pSrcData + r * cbSrcRow + (width - 1) * cbSrcInc) = 0x00;
    }
    std::fill(pSrcData, pSrcData + (width - 1) * cbSrcInc, 0x00);
    std::fill(pSrcData + (height - 1) * cbSrcRow,
              pSrcData + (height - 1) * cbSrcRow + (width - 1) * cbSrcInc,
              0x00);

    static const int x[] = {
        1, 1, 0, -1, -1, -1, 0, 1,
    };
    static const int y[] = {
        0, 1, 1, 1, 0, -1, -1, -1,
    };
    for (int r = 1; r < height - 1; ++r) {
      for (int c = 1; c < width - 1; ++c) {
        auto pixel = pDstData + r * cbDestRow + cbDestInc * c;
        auto srcpixel = pSrcData + r * cbSrcRow + cbSrcInc * c;
        int count = 0;
        for (int i = 0; i < 8; ++i) {
          auto ptr = srcpixel + x[i] * cbSrcInc + y[i] * cbSrcRow;
          if (*ptr == ALIVE_COLOR)
            ++count;
        }

        switch (count) {
        case 3:
          *pixel = ALIVE_COLOR;
          break;

        case 2:
          *pixel = *srcpixel;
          break;

        default:
          *pixel = 0x00;
          break;
        }
      }
    }
  }
};
