#pragma once

#include <stdint.h>

//#ifdef __cplusplus

void *aligned_malloc_impl(int size, int alignment);
void aligned_free(void *ptr);

#ifdef __cplusplus
template <typename T>
T* aligned_malloc(int size, int alignment) {
	return reinterpret_cast<T*>(aligned_malloc_impl(size, alignment));
}
#else
#define aligned_malloc aligned_malloc_impl
#endif // __cplusplus

void ui8vadd(int n, const uint8_t *lhs, const uint8_t *rhs, uint8_t *dst);
void ui8vaddu(int n, const uint8_t *lhs, const uint8_t *rhs, uint8_t *dst);

enum ivOp {
	ivEq = 0, //_MM_CMP_INT_EQ
};

void ui8vcmpcu(int n, const uint8_t* a, uint8_t* x, uint8_t c, ivOp op);

void ui8vandu(int n, const uint8_t* a, const uint8_t* b, uint8_t* x);
void ui8voru(int n, const uint8_t* a, const uint8_t* b, uint8_t* x);

void ui8vandcu(int n, const uint8_t* a, uint8_t* x, uint8_t c);

//#endif // __cplusplus
