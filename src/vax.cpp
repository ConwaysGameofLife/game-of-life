#include "vax.h"

#include <immintrin.h>
#include <cstring>
#include <algorithm>

#ifdef NO_IPP

extern "C" {

IppStatus ippInit() { return ippStsNoErr; }

IppStatus ippiAndC_8u_C1R(const uint8_t *pSrc, int srcStep, const uint8_t value,
                          uint8_t *pDst, int dstStep, IppiSize roiSize) {
  for (int r = 0; r < roiSize.height; ++r) {
    auto srcptr = pSrc + r * srcStep;
    auto dstptr = pDst + r * dstStep;
    for (int c = 0; c < roiSize.width; ++c) {
      dstptr[c] = srcptr[c] & value;
    }
  }
  return ippStsNoErr;
}

IppStatus ippiAnd_8u_C1R(const uint8_t *pSrc1, int src1Step,
                         const uint8_t *pSrc2, int src2Step, uint8_t *pDst,
                         int dstStep, IppiSize roiSize) {
  for (int r = 0; r < roiSize.height; ++r) {
    auto src1ptr = pSrc1 + r * src1Step;
    auto src2ptr = pSrc2 + r * src2Step;
    auto dstptr = pDst + r * dstStep;
    for (int c = 0; c < roiSize.width; ++c) {
      dstptr[c] = src1ptr[c] & src2ptr[c];
    }
  }
  return ippStsNoErr;
}

IppStatus ippiOr_8u_C1R(const uint8_t *pSrc1, int src1Step,
                        const uint8_t *pSrc2, int src2Step, uint8_t *pDst,
                        int dstStep, IppiSize roiSize) {
  for (int r = 0; r < roiSize.height; ++r) {
    auto src1ptr = pSrc1 + r * src1Step;
    auto src2ptr = pSrc2 + r * src2Step;
    auto dstptr = pDst + r * dstStep;
    for (int c = 0; c < roiSize.width; ++c) {
      dstptr[c] = src1ptr[c] | src2ptr[c];
    }
  }
  return ippStsNoErr;
}

IppStatus ippiSet_8u_C1R(uint8_t value, uint8_t *pDst, int dstStep,
                         IppiSize roiSize) {
  for (int r = 0; r < roiSize.height; ++r) {
    auto ptr = pDst + r * dstStep;
    std::fill(ptr, ptr + sizeof(uint8_t) * roiSize.width, value);
  }
  return ippStsNoErr;
}

IppStatus ippiAdd_8u_C1RSfs(const uint8_t *pSrc1, int src1Step,
                            const uint8_t *pSrc2, int src2Step, uint8_t *pDst,
                            int dstStep, IppiSize roiSize, int scaleFactor) {
  for (int r = 0; r < roiSize.height; ++r) {
    auto src1ptr = pSrc1 + r * src1Step;
    auto src2ptr = pSrc2 + r * src2Step;
    auto dstptr = pDst + r * dstStep;
    for (int c = 0; c < roiSize.width; ++c) {
      dstptr[c] = (src1ptr[c] + src2ptr[c]) >> scaleFactor;
    }
  }
  return ippStsNoErr;
}

IppStatus ippiCopy_8u_C1R(const uint8_t *pSrc, int srcStep, uint8_t *pDst,
                          int dstStep, IppiSize roiSize) {
  for (int r = 0; r < roiSize.height; ++r) {
    auto srcptr = pSrc + r * srcStep;
    auto dstptr = pDst + r * dstStep;
    memcpy(dstptr, srcptr, sizeof(uint8_t) * roiSize.width);
  }
  return ippStsNoErr;
}

IppStatus ippiCompareC_8u_C1R(const uint8_t *pSrc, int srcStep, uint8_t value,
                              uint8_t *pDst, int dstStep, IppiSize roiSize,
                              IppCmpOp ippCmpOp) {
  for (int r = 0; r < roiSize.height; ++r) {
    auto srcptr = pSrc + r * srcStep;
    auto dstptr = pDst + r * dstStep;
    for (int c = 0; c < roiSize.width; ++c) {
      dstptr[c] = srcptr[c] == value ? 0xFF : 0x00;
    }
  }
  return ippStsNoErr;
}

}

#endif // NO_IPP
