#ifndef J64__H
#define J64__H

#ifdef J64_STATIC
#define J64_STATIC_API static
#else
#define J64_STATIC_API
#endif /* J64_STATIC */

#ifdef J64_INLINE
#define J64_INLINE_API inline
#else
#define J64_INLINE_API
#endif /* J64_INLINE */

#define J64_API J64_STATIC_API J64_INLINE_API

#ifdef J64_DEBUG
#include <assert.h>
#define j64__assert(x) assert(x)
#else
#define j64__assert(x)
#endif

#ifndef J64_MALLOC
#include <stdlib.h>
#define J64_MALLOC malloc
#endif /* J64_MALLOC */

#ifndef J64_REALLOC
#include <stdlib.h>
#define J64_REALLOC realloc
#endif

#ifndef J64_FREE
#include <stdlib.h>
#define J64_FREE free
#endif

#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* J64 union type for different types of accesses */
typedef union {
	uint64_t	w;
	int64_t		i;
	double		f;
	uint8_t		b[8];
	uintptr_t	p;
} j64_t;

#define J64__INIT { 0 }

/*
 * Primary types
 */

#define J64__TYPE_OFFS		0
#define J64__TYPE_SIZE		3
#define J64__TYPE_MASK		((1 << J64__TYPE_SIZE) - 1)
#define J64__PTR_MASK		(~J64__TYPE_MASK)

#define J64_TYPE_LIT		0x0
#define J64_TYPE_FLOAT		0x1
#define J64_TYPE_ISTR		0x2
#define J64_TYPE_INT0		0x3
#define J64_TYPE_BSTR		0x4
#define J64_TYPE_BARR		0x5
#define J64_TYPE_OBJ		0x6
#define J64_TYPE_INT1		0x7

#define J64_TYPE_GET(j)		((j).w & J64__TYPE_MASK)

/* Forward declaration */
J64_API void j64_free(j64_t);

/*
 * Literal subtype constants and functions
 */

#define J64__TYPE_LIT_OFFS	(J64__TYPE_OFFS + J64__TYPE_SIZE)
#define J64__TYPE_LIT_SIZE	(J64__TYPE_LIT_OFFS + 3)
#define J64__TYPE_LIT_MASK	((1 << J64__TYPE_LIT_SIZE) - 1)

/* undefined is all zero by design */
#define J64_TYPE_LIT_UNDEF	((0x0 << J64__TYPE_LIT_OFFS) | J64_TYPE_LIT)
#define J64_TYPE_LIT_NULL	((0x1 << J64__TYPE_LIT_OFFS) | J64_TYPE_LIT)
#define J64_TYPE_LIT_FALSE	((0x2 << J64__TYPE_LIT_OFFS) | J64_TYPE_LIT)
#define J64_TYPE_LIT_TRUE	((0x3 << J64__TYPE_LIT_OFFS) | J64_TYPE_LIT)
#define J64_TYPE_LIT_ESTR	((0x4 << J64__TYPE_LIT_OFFS) | J64_TYPE_LIT)
#define J64_TYPE_LIT_EARR	((0x5 << J64__TYPE_LIT_OFFS) | J64_TYPE_LIT)
#define J64_TYPE_LIT_EOBJ	((0x6 << J64__TYPE_LIT_OFFS) | J64_TYPE_LIT)
#define J64_TYPE_LIT_DEL	((0x7 << J64__TYPE_LIT_OFFS) | J64_TYPE_LIT)

#define J64_TYPE_LIT_GET(j)	((j).w & J64__TYPE_LIT_MASK)

J64_API j64_t
j64_undef(void)
{
	j64_t j = J64__INIT;
	j.w = J64_TYPE_LIT_UNDEF;
	return j;
}

J64_API int
j64_is_undef(j64_t j)
{
	return j.w == J64_TYPE_LIT_UNDEF;
}

J64_API j64_t
j64_null(void)
{
	j64_t j = J64__INIT;
	j.w = J64_TYPE_LIT_NULL;
	return j;
}

J64_API int
j64_is_null(j64_t j)
{
	return j.w == J64_TYPE_LIT_NULL;
}

J64_API j64_t
j64_false(void)
{
	j64_t j = J64__INIT;
	j.w = J64_TYPE_LIT_FALSE;
	return j;
}

J64_API j64_t
j64_true(void)
{
	j64_t j = J64__INIT;
	j.w = J64_TYPE_LIT_TRUE;
	return j;
}

J64_API j64_t
j64_bool(int b)
{
	j64_t j = J64__INIT;
	j.w = b ? J64_TYPE_LIT_TRUE : J64_TYPE_LIT_FALSE;
	return j;
}

J64_API int
j64_is_true(j64_t j)
{
	return j.w == J64_TYPE_LIT_TRUE;
}

J64_API int
j64_is_false(j64_t j)
{
	return j.w == J64_TYPE_LIT_FALSE;
}

J64_API j64_t
j64_estr(void)
{
	j64_t j = J64__INIT;
	j.w = J64_TYPE_LIT_ESTR;
	return j;
}

J64_API int
j64_is_estr(j64_t j)
{
	return j.w == J64_TYPE_LIT_ESTR;
}

J64_API j64_t
j64_earr(void)
{
	j64_t j = J64__INIT;
	j.w = J64_TYPE_LIT_EARR;
	return j;
}

J64_API int
j64_is_earr(j64_t j)
{
	return j.w == J64_TYPE_LIT_EARR;
}

/*
 * Integer contants and functions
 */

#define J64__INT_OFFS		(J64__TYPE_SIZE - 1)

#define J64_INT_MIN             (-0x1fffffffffffffffLL - 1)
#define J64_INT_MAX             (0x1fffffffffffffffLL)

J64_API j64_t
j64_int(int64_t i)
{
	j64_t j = J64__INIT;

	j64__assert(J64_INT_MIN <= i && i <= J64_INT_MAX);

	j.i = i << J64__INT_OFFS;
	j.w |= J64_TYPE_INT0;

	return j;
}

J64_API int
j64_is_int(j64_t j)
{
	return J64_TYPE_GET(j) == J64_TYPE_INT0 || J64_TYPE_GET(j) == J64_TYPE_INT1;
}

J64_API int64_t
j64_int_get(j64_t j)
{
	j64__assert(j64_is_int(j));

	return j.i >> J64__INT_OFFS;
}

/*
 * Floats
 */

/* TODO: J64-specific min-max float constants */
#define J64__FLOAT_OFFS 3
#define J64__FLOAT_MASK 0xfffffffffffffff8

J64_API j64_t
j64_float(double f)
{
	j64_t j = J64__INIT;

	j.f = f;
	j.w &= J64__FLOAT_MASK; /* zero the lowest mantissa bits for type */
	j.w |= J64_TYPE_FLOAT;

	return j;
}

J64_API int
j64_is_float(j64_t j)
{
	return J64_TYPE_GET(j) == J64_TYPE_FLOAT;
}

J64_API double
j64_float_get(j64_t j)
{
	j64__assert(j64_is_float(j));
	j.w &= J64__FLOAT_MASK;
	return j.f;
}

/*
 * Immediate string constants and functions
 */

#define J64__ISTR_LEN_OFFS	(J64__TYPE_OFFS + J64__TYPE_SIZE)
#define J64__ISTR_LEN_SIZE	3
#define J64__ISTR_LEN_MASK	(((1 << J64__ISTR_LEN_SIZE) - 1) << J64__ISTR_LEN_OFFS)

#define J64_ISTR_LEN_MAX	7

J64_API j64_t
j64_istr(const void *buf, size_t len)
{
	j64_t j = J64__INIT;

	len = len < J64_ISTR_LEN_MAX ? len : J64_ISTR_LEN_MAX;
	memcpy(&j.b[1], buf, len);
	j.w |= (len << J64__ISTR_LEN_OFFS) & J64__ISTR_LEN_MASK;
	j.w |= J64_TYPE_ISTR;

	return j;
}

J64_API int
j64_is_istr(j64_t j)
{
	return J64_TYPE_GET(j) == J64_TYPE_ISTR;
}

J64_API size_t
j64_istr_len(j64_t j)
{
	return (j.w & J64__ISTR_LEN_MASK) >> J64__ISTR_LEN_OFFS;
}

J64_API size_t
j64_istr_get(j64_t j, void *buf, size_t len)
{
	size_t n;

	j64__assert(j64_is_istr(j));
	n = j64_istr_len(j);
	n = n < len ? n : len;
	memcpy(buf, &j.b[1], n);

	return n;
}

/*
 * Boxed string
 */

struct j64__bstr_hdr {
	size_t	len;
	uint8_t	buf;
};

#define J64__BSTR_HDR(j)	((struct j64__bstr_hdr *)((j).p & (uintptr_t)J64__PTR_MASK))
#define J64__BSTR_HDR_SIZEOF	(offsetof(struct j64__bstr_hdr, buf))

J64_API j64_t
j64_bstr(const void *buf, size_t len)
{
	j64_t j = J64__INIT;
	struct j64__bstr_hdr *hdr;

	j64__assert(buf != NULL);
	j64__assert(len < SIZE_MAX - J64__BSTR_HDR_SIZEOF);

	hdr = J64_MALLOC(J64__BSTR_HDR_SIZEOF + len);
	if (hdr == NULL)
		return j64_undef();

	hdr->len = len;
	memcpy(&hdr->buf, buf, len);

	j.p = (uintptr_t)hdr;
	j.w |= J64_TYPE_BSTR;

	return j;
}

J64_API int
j64_is_bstr(j64_t j)
{
	return J64_TYPE_GET(j) == J64_TYPE_BSTR;
}

J64_API size_t
j64_bstr_len(j64_t j)
{
	struct j64__bstr_hdr *hdr;
	j64__assert(j64_is_bstr(j));
	hdr = J64__BSTR_HDR(j);

	return hdr->len;
}

J64_API size_t
j64_bstr_get(j64_t j, void *buf, size_t len)
{
	struct j64__bstr_hdr *hdr;
	size_t n;

	j64__assert(j64_is_bstr(j));

	hdr = J64__BSTR_HDR(j);
	n = j64_bstr_len(j);
	n = n < len ? n : len;
	memcpy(buf, &hdr->buf, n);

	return n;
}

J64_API void
j64_bstr_free(j64_t j)
{
	j64__assert(j64_is_bstr(j));
	J64_FREE(J64__BSTR_HDR(j));
}

/*
 * Boxed array
 */

struct j64__barr_hdr {
	size_t	cap;
	j64_t	buf;
};

#define J64__BARR_HDR(j)	((struct j64__barr_hdr *)((j).p & (uintptr_t)J64__PTR_MASK))
#define J64__BARR_HDR_SIZEOF	(offsetof(struct j64__barr_hdr, buf))
#define J64__BARR_HDR_CAP_MAX	((SIZE_MAX - J64__BARR_HDR_SIZEOF) / sizeof(j64_t))

#define J64_BARR_CAP_MAX	J64__BARR_HDR_CAP_MAX

J64_API j64_t
j64_barr_alloc(size_t cap)
{
	j64_t j = J64__INIT;
	struct j64__barr_hdr *hdr;
	size_t i;

	/* Overflow check */
	if (J64__BARR_HDR_CAP_MAX < cap)
		return j64_undef();

	hdr = malloc(J64__BARR_HDR_SIZEOF + cap * sizeof(j64_t));
	if (hdr == NULL)
		return j64_undef();

	hdr->cap = cap;
	/* TODO: remove initialization? */
	for (i = 0; i < cap; i++)
		(&hdr->buf)[i] = j64_undef();

	j.p = (uintptr_t)hdr;
	j.w |= J64_TYPE_BARR;

	return j;
}

/*
 * Reallocates an array with a new capacity.
 * If the new capacity is smaller than the old one,
 * truncates the array WITHOUT freeing the values
 * at the end of the old array.
 *
 * Returns the new array on success, undefined otherwise.
 */
J64_API j64_t
j64_barr_realloc(j64_t j, size_t new_cap)
{
	struct j64__barr_hdr *hdr, *new_hdr;
	size_t cap;
	size_t new_size;
	size_t i;

	j64__assert(j64_is_barr(j));

	/* Overflow check */
	if (J64__BARR_HDR_CAP_MAX < new_cap)
		return j64_undef();

	hdr = J64__BARR_HDR(j);
	cap = hdr->cap;
	new_size = J64__BARR_HDR_SIZEOF + new_cap * sizeof(j64_t);
	new_hdr = realloc(hdr, new_size);
	if (new_hdr == NULL)
		return j64_undef();

	/* Initialize(TODO: remove?) the grown area if new capacity is greater */
	for (i = cap; i < new_cap; i++)
		(&new_hdr->buf)[i] = j64_undef();

	new_hdr->cap = new_cap;
	j.p = (uintptr_t)new_hdr;
	j.w |= J64_TYPE_BARR;

	return j;
}

J64_API int
j64_is_barr(j64_t j)
{
	return J64_TYPE_GET(j) == J64_TYPE_BARR;
}

J64_API size_t
j64_barr_cap(j64_t j)
{
	struct j64__barr_hdr *hdr;
	j64__assert(j64_is_barr(j));
	hdr = J64__BARR_HDR(j);
	return hdr->cap;
}

J64_API j64_t
j64_barr_get(j64_t j, size_t i)
{
	struct j64__barr_hdr *hdr;

	j64__assert(j64_is_barr(j));
	hdr = J64__BARR_HDR(j);
	j64__assert(i < hdr->cap;);

	return (&hdr->buf)[i];
}

J64_API void
j64_barr_set(j64_t j, j64_t k, size_t i)
{
	struct j64__barr_hdr *hdr;

	j64__assert(j64_is_barr(j));
	hdr = J64__BARR_HDR(j);
	j64__assert(i < hdr->cap);
	(&hdr->buf)[i] = k;
}

J64_API void
j64_barr_set_free(j64_t j, j64_t k, size_t i)
{
	struct j64__barr_hdr *hdr;

	j64__assert(j64_is_barr(j));
	hdr = J64__BARR_HDR(j);
	j64__assert(i < hdr->cap);
	j64_free((&hdr->buf)[i]);
	(&hdr->buf)[i] = k;
}

J64_API void
j64_barr_free(j64_t j)
{
	j64__assert(j64_is_barr(j));
	J64_FREE(J64__BARR_HDR(j));
}

/*
 * Polymorphic free
 */
J64_API void
j64_free(j64_t j)
{
	switch (J64_TYPE_GET(j)) {
	case J64_TYPE_BSTR:
		j64_bstr_free(j);
		break;
	case J64_TYPE_BARR:
		j64_barr_free(j);
		break;
	}
}

/*
 * misc
 */

#ifdef J64_DEBUG
J64_API void
j64_dbg(j64_t j)
{
#include <stdio.h>
#include <inttypes.h>

	const char *lit;
	char istr[J64_ISTR_LEN_MAX + 1];

	memset(istr, '\0', sizeof(istr));

	switch (J64_TYPE_GET(j)) {
	case J64_TYPE_LIT:
		switch (J64_TYPE_LIT_GET(j)) {
		case J64_TYPE_LIT_UNDEF:
			lit = "undefined";
			break;
		case J64_TYPE_LIT_NULL:
			lit = "null";
			break;
		case J64_TYPE_LIT_FALSE:
			lit = "false";
			break;
		case J64_TYPE_LIT_TRUE:
			lit = "true";
			break;
		case J64_TYPE_LIT_ESTR:
			lit = "empty string";
			break;
		case J64_TYPE_LIT_EARR:
			lit = "empty array";
			break;
		case J64_TYPE_LIT_EOBJ:
			lit = "empty object";
			break;
		case J64_TYPE_LIT_DEL:
			lit = "deleted";
			break;
		default:
			lit = "unknown";
		}
		fprintf(stderr, "literal: %s\n", lit);
		break;
	case J64_TYPE_INT0:
	case J64_TYPE_INT1:
		fprintf(stderr, "integer: %" PRIx64 "\n", j64_int_get(j));
		break;
	case J64_TYPE_ISTR:
		memcpy(istr, &j.b[1], j64_istr_len(j));
		fprintf(stderr, "immediate string: %zu \"%s\"\n",
		    j64_istr_len(j), istr);
		break;
	default:
		fprintf(stderr, "unknown\n");
		break;
	}
}
#else
J64_API void
j64_dbg(j64_t j)
{
	(void)j;
}
#endif /* J64_DEBUG */

#endif
