#ifndef __SCALE_H__
#define __SCALE_H__

#include <string.h>
#include "types.h"

#define BIT(nr)			(1UL << (nr))
#define BIT_MASK(nr)		(1UL << ((nr) % BITS_PER_LONG))
#define BIT_WORD(nr)		((nr) / BITS_PER_LONG)

#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))

#define BITS_TO_LONGS(nr)	DIV_ROUND_UP(nr, BITS_PER_LONG)

#define BITMAP_FIRST_WORD_MASK(start) (~0UL << ((start) % BITS_PER_LONG))
#define BITMAP_LAST_WORD_MASK(nbits)					\
(									\
	((nbits) % BITS_PER_LONG) ?					\
		(1UL<<((nbits) % BITS_PER_LONG))-1 : ~0UL		\
)
#define small_const_nbits(nbits) \
	((nbits) <= BITS_PER_LONG)

#define ALIGN_BITS	(10)
#define PAGE_SIZE	(1 << ALIGN_BITS)
#define ALIGN_PAGE(x)	(((x) + ((1 << ALIGN_BITS) - 1)) & (~((1 << ALIGN_BITS) - 1)))
#define LENS_TO_BITS(x) (ALIGN_PAGE(x) >> ALIGN_BITS)

static inline int get_order(unsigned long size)
{
    int order = 0;

    do{
        size >>= 1;
        order++;
    }while(size);

    return order;
}

static inline void set_bit(u32 n, unsigned long *bits){
    bits[n >> get_order(BITS_PER_LONG - 1)] |= (1 << (n & (BITS_PER_LONG - 1)));
}

static inline void clear_bit(u32 n, unsigned long *bits){
    bits[n >> get_order(BITS_PER_LONG - 1)] &= ~((1 << (n & (BITS_PER_LONG - 1))));
}

static inline int test_bit(u32 n, unsigned long *bits){
    return ((bits[n >> get_order(BITS_PER_LONG -1 )] & (1 << (n & (BITS_PER_LONG - 1)))) != 0);
}

static inline void bitmap_zero(unsigned long *dst, int nbits)
{
    if (small_const_nbits(nbits))
        *dst = 0UL;
    else {
        int len = BITS_TO_LONGS(nbits) * sizeof(unsigned long);
        memset(dst, 0, len);
	}
}

static inline void bitmap_fill(unsigned long *dst, int nbits)
{
	size_t nlongs = BITS_TO_LONGS(nbits);
	if (!small_const_nbits(nbits)) {
		size_t len = (nlongs - 1) * sizeof(unsigned long);
		memset(dst, 0xff,  len);
	}
	dst[nlongs - 1] = BITMAP_LAST_WORD_MASK(nbits);
}

static inline void __bitmap_shift_left(unsigned long *dst,
			const unsigned long *src, int shift, int bits)
{
	int k, lim = BITS_TO_LONGS(bits), left = bits % BITS_PER_LONG;
	int off = shift/BITS_PER_LONG, rem = shift % BITS_PER_LONG;
	for (k = lim - off - 1; k >= 0; --k) {
		unsigned long upper, lower;

		/*
		 * If shift is not word aligned, take upper rem bits of
		 * word below and make them the bottom rem bits of result.
		 */
		if (rem && k > 0)
			lower = src[k - 1];
		else
			lower = 0;
		upper = src[k];
		if (left && k == lim - 1)
			upper &= (1UL << left) - 1;
		dst[k + off] = lower  >> (BITS_PER_LONG - rem) | upper << rem;
		if (left && k + off == lim - 1)
			dst[k + off] &= (1UL << left) - 1;
	}
	if (off)
		memset(dst, 0, off*sizeof(unsigned long));
}

static inline void shift_left_bit(u32 n, unsigned long *bits, u32 nbits){
    if (small_const_nbits(nbits))
        *bits = (*bits << n) & BITMAP_LAST_WORD_MASK(nbits);
    else
        __bitmap_shift_left(bits, bits, n, nbits);

}

static inline void __bitmap_shift_right(unsigned long *dst,
			const unsigned long *src, int shift, int bits)
{
	int k, lim = BITS_TO_LONGS(bits), left = bits % BITS_PER_LONG;
	int off = shift/BITS_PER_LONG, rem = shift % BITS_PER_LONG;
	unsigned long mask = (1UL << left) - 1;
	for (k = 0; off + k < lim; ++k) {
		unsigned long upper, lower;

		/*
		 * If shift is not word aligned, take lower rem bits of
		 * word above and make them the top rem bits of result.
		 */
		if (!rem || off + k + 1 >= lim)
			upper = 0;
		else {
			upper = src[off + k + 1];
			if (off + k + 1 == lim - 1 && left)
				upper &= mask;
		}
		lower = src[off + k];
		if (left && off + k == lim - 1)
			lower &= mask;
		dst[k] = upper << (BITS_PER_LONG - rem) | lower >> rem;
		if (left && k == lim - 1)
			dst[k] &= mask;
	}
	if (off)
		memset(&dst[lim - off], 0, off*sizeof(unsigned long));
}

static inline void shift_right_bit(u32 n, unsigned long *bits, u32 nbits){
    if (small_const_nbits(nbits))
        *bits = *bits >> n;
    else
        __bitmap_shift_right(bits, bits, n, nbits);
}

void gen_bits_map(unsigned long *fap_map,int src_fps,int target_fps, int maxbits);
#endif //__SCALE_H__
