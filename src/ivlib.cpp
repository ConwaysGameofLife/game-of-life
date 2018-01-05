#include "ivlib.h"

#include <emmintrin.h>
#include <assert.h>

void *aligned_malloc_impl(int size, int alignment) {
  assert(alignment == 4 || alignment == 8 || alignment == 16 ||
         alignment == 32);
  assert(size > 0);

  const int payload_size = sizeof(intptr_t);
  auto alloc_size = alignment - 1 + payload_size + size;
  auto origin = new uint8_t[alloc_size];
  if (!origin)
    return origin;
  auto ptr = reinterpret_cast<uintptr_t>(origin + payload_size);
  ptr += (alignment - ptr % alignment) % alignment;
  assert(ptr % alignment == 0);
  ((void **)ptr)[-1] = origin;
  return reinterpret_cast<void *>(ptr);
}

void aligned_free(void *ptr) {
  if (!ptr)
    return;
  void *origin = static_cast<void **>(ptr)[-1];
  delete origin;
}

void ui8vadd(int n, const uint8_t *lhs, const uint8_t *rhs, uint8_t *dst) {
  const int step = 128 / 8;
  int i = 0;
  for (; i < n - step; i += step) {
    __m128i l = _mm_load_si128((const __m128i *)&lhs[i]);
    __m128i r = _mm_load_si128((const __m128i *)&rhs[i]);
    __m128i s = _mm_add_epi8(l, r);
    _mm_store_si128((__m128i *)&dst[i], s);
  }
  for (; i < n; ++i) {
    dst[i] = lhs[i] + rhs[i];
  }
}

void ui8vaddu(int n, const uint8_t *lhs, const uint8_t *rhs, uint8_t *dst) {
  const int step = 128 / 8;
  int i = 0;
  for (; i < n - step; i += step) {
    __m128i l = _mm_loadu_si128((const __m128i *)&lhs[i]);
    __m128i r = _mm_loadu_si128((const __m128i *)&rhs[i]);
    __m128i s = _mm_add_epi8(l, r);
    _mm_storeu_si128((__m128i *)&dst[i], s);
  }
  for (; i < n; ++i) {
    dst[i] = lhs[i] + rhs[i];
  }
}

void ui8vcmpcu(int n, const uint8_t *a, uint8_t *x, uint8_t c, ivOp op) {
  const int step = 128 / 8;
  int i = 0;
  const __m128i mc = _mm_set1_epi8(c);
  for (; i < n - step; i += step) {
    __m128i ma = _mm_loadu_si128((const __m128i *)&a[i]);
    __m128i mx = _mm_cmpeq_epi8(ma, mc);
    _mm_storeu_si128((__m128i *)&x[i], mx);
  }
  for (; i < n; ++i) {
    x[i] = a[i] == c ? 0xFF : 0x00;
  }
}

void ui8vandu(int n, const uint8_t *a, const uint8_t *b, uint8_t *x) {
  const int step = 128 / 8;
  int i = 0;
  for (; i < n - step; i += step) {
    __m128i ma = _mm_loadu_si128((const __m128i *)&a[i]);
    __m128i mb = _mm_loadu_si128((const __m128i *)&b[i]);
    __m128i mx = _mm_and_si128(ma, mb);
    _mm_storeu_si128((__m128i *)&x[i], mx);
  }
  for (; i < n; ++i) {
    x[i] = a[i] & b[i];
  }
}

void ui8voru(int n, const uint8_t *a, const uint8_t *b, uint8_t *x) {
  const int step = 128 / 8;
  int i = 0;
  for (; i < n - step; i += step) {
    __m128i ma = _mm_loadu_si128((const __m128i *)&a[i]);
    __m128i mb = _mm_loadu_si128((const __m128i *)&b[i]);
    __m128i mx = _mm_or_si128(ma, mb);
    _mm_storeu_si128((__m128i *)&x[i], mx);
  }
  for (; i < n; ++i) {
    x[i] = a[i] | b[i];
  }
}

void ui8vandcu(int n, const uint8_t *a, uint8_t *x, uint8_t c) {
  const int step = 128 / 8;
  int i = 0;
  const __m128i mc = _mm_set1_epi8(c);
  for (; i < n - step; i += step) {
    __m128i ma = _mm_loadu_si128((const __m128i *)&a[i]);
    __m128i mx = _mm_and_si128(ma, mc);
    _mm_storeu_si128((__m128i *)&x[i], mx);
  }
  for (; i < n; ++i) {
    x[i] = a[i] & c;
  }
}
