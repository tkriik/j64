#ifndef _J64_H_
#define _J64_H_

/*
 * J64 library type definitions, macros and function declarations.
 * This library stores every JSON value as a 8-byte word, where
 * the least significant bits indicate the type of the value and
 * whether the word contains the direct value (immediate) or contains
 * a pointer to a heap-allocated value (boxed).
 * 
 * Immediate values can include:
 *   - literal values, which include:
 *     - null
 *     - true, false
 *     - empty string, object or array
 *   - 62-bit integers
 *   - 61-bit floating point numbers
 *     (with 3 least significant bits in mantissa zeroed)
 *   - short strings (with length less or equal to 7 bytes)
 *
 * Boxed values can include:
 *   - long strings (with length over 7 bytes)
 *   - non-empty arrays
 *   - non-empty objects
 */

#include <stdint.h>
#include <stddef.h>

typedef uint64_t	 _j64_word_t;
typedef int64_t		 _j64_int_t;
typedef double		 _j64_float_t;
typedef uint8_t		 _j64_byte_t;
typedef void		*_j64_ptr_t;

/*
 * J64 union type. The unsigned integer word (w) is
 * used to perform bit manipulations, while other fields
 * are used for differnet types of accesses.
 */
typedef union {
	_j64_word_t	w;
	_j64_int_t	i;
	_j64_float_t	f;
	_j64_byte_t	b[8];
	_j64_ptr_t	p;
} j64_t;

/* 
 * The three least significant bits form a primary tag that denotes
 * how the next 61 bits should be interpreted, which can be one
 * of the following:
 *   - immediate literal
 *   - immediate integer
 *   - immediate floating-point number
 *   - immediate short string
 *   - boxed long string
 *   - boxed array
 *   - boxed object
 *
 * Two bit sequences denote an integer, which are 011 (3) and 111 (7).
 * This way we can get one free extra bit when storing integers (thanks vihtori).
 */
#define _J64_TAG_PRIM_SIZE	3
#define _J64_TAG_PRIM_MASK	((1 << _J64_TAG_PRIM_SIZE) - 1)
#define _J64_TAG_PRIM_INT_SIZE	(_J64_TAG_PRIM_SIZE - 1)
#define _J64_TAG_PRIM_INT_MASK	((1 << _J64_TAG_PRIM_INT_SIZE) - 1)

#define J64_TAG_PRIM_LIT	0x0
#define J64_TAG_PRIM_FLOAT	0x1
#define J64_TAG_PRIM_ISTR	0x2
#define J64_TAG_PRIM_INT0	0x3
#define J64_TAG_PRIM_BSTR	0x4
#define J64_TAG_PRIM_ARR	0x5
#define J64_TAG_PRIM_OBJ	0x6
#define J64_TAG_PRIM_INT1	0x7

#define j64_prim_tag(j)	((j).w & _J64_TAG_PRIM_MASK)
#define _j64_prim_tag_set(j, t)	((j).w |= (t))

#define _j64_get_ptr(j)		((_j64_ptr_t)((j).w & ~_J64_TAG_PRIM_MASK))

#define J64_UINT_MAX		0x3fffffffffffffffULL
#define J64_INT_MIN		(-0x1fffffffffffffffLL - 1)
#define J64_INT_MAX		0x1fffffffffffffffLL

/*
 * 3 bits after a primary tag of J64_TAG_PRIM_LIT form a literal tag,
 * which denotes the type of a literal value stored. A literal value
 * can be one of the following:
 *   - null
 *   - false
 *   - true
 *   - empty string
 *   - empty array
 *   - empty object
 */
#define J64_SUBTAG_LIT_OFFS	_J64_TAG_PRIM_SIZE
#define J64_SUBTAG_LIT_SIZE	(3 + J64_SUBTAG_LIT_OFFS)
#define J64_SUBTAG_LIT_MASK	((1 << J64_SUBTAG_LIT_SIZE) - 1)

#define J64_SUBTAG_LIT_NULL	((0x0 << J64_SUBTAG_LIT_OFFS) | J64_TAG_PRIM_LIT)
#define J64_SUBTAG_LIT_FALSE	((0x1 << J64_SUBTAG_LIT_OFFS) | J64_TAG_PRIM_LIT)
#define J64_SUBTAG_LIT_TRUE	((0x2 << J64_SUBTAG_LIT_OFFS) | J64_TAG_PRIM_LIT)
#define J64_SUBTAG_LIT_ESTR	((0x3 << J64_SUBTAG_LIT_OFFS) | J64_TAG_PRIM_LIT)
#define J64_SUBTAG_LIT_EARR	((0x4 << J64_SUBTAG_LIT_OFFS) | J64_TAG_PRIM_LIT)
#define J64_SUBTAG_LIT_EOBJ	((0x5 << J64_SUBTAG_LIT_OFFS) | J64_TAG_PRIM_LIT)

/*
 * Special values to mark non-values and deleted values in an object.
 * These can be embedded in the literal tag since it leaves two unused
 * values.
 */
#define _J64_SUBTAG_LIT_NON	((0x6 << J64_SUBTAG_LIT_OFFS) | J64_TAG_PRIM_LIT)
#define _J64_SUBTAG_LIT_DEL	((0x7 << J64_SUBTAG_LIT_OFFS) | J64_TAG_PRIM_LIT)

#define j64_subtag_lit(j)	((j).w & J64_SUBTAG_LIT_MASK)

/*
 * 3 bits after a primary tag of J64_TAG_PRIM_ISTR contain a length
 * value for a short immediate string.
 */
#define _J64_SUBTAG_STR_LEN_OFFS _J64_TAG_PRIM_SIZE
#define _J64_SUBTAG_STR_LEN_SIZE (3 + _J64_SUBTAG_STR_LEN_OFFS)
#define _J64_SUBTAG_STR_LEN_MASK ((1 << _J64_SUBTAG_STR_LEN_SIZE) - 1)

#define j64_istr_len(j)							\
    (((j).w & _J64_SUBTAG_STR_LEN_MASK) >> _J64_SUBTAG_STR_LEN_OFFS)
#define _j64_istr_len_set(j, n)						\
    ((j).w |= (((n) & _J64_SUBTAG_STR_LEN_MASK) << _J64_SUBTAG_STR_LEN_OFFS))

/* Boxed string memory header. */
struct _j64_bstr_hdr {
	_j64_word_t len;
	_j64_byte_t buf; /* used only for addressing */
};

#define _j64_bstr_hdr(j)	((struct _j64_bstr_hdr *)_j64_get_ptr(j))
#define _J64_BSTR_HDR_SIZEOF	(sizeof(struct _j64_bstr_hdr) - 1)
#define j64_bstr_len(j)		(((struct _j64_bstr_hdr *)_j64_get_ptr(j))->len)
#define _j64_bstr_buf(j)	(&(((struct _j64_bstr_hdr *)_j64_get_ptr(j))->buf))

/* Boxed array memory header. */
struct _j64_arr_hdr {
	_j64_word_t cnt;
	_j64_word_t cap;
	_j64_byte_t buf; /* used only for addressing */
};

#define _J64_ARR_HDR_SIZEOF	(sizeof(struct _j64_arr_hdr) - 1)

#define _j64_arr_hdr(j)		((struct _j64_arr_hdr *)_j64_get_ptr(j))
#define j64_arr_cnt(j)		(((struct _j64_arr_hdr *)_j64_get_ptr(j))->cnt)
#define _j64_arr_cap(j)		(((struct _j64_arr_hdr *)_j64_get_ptr(j))->cap)
#define _j64_arr_buf(j)		((j64_t *)&(((struct _j64_arr_hdr *)_j64_get_ptr(j))->buf))

/* Constructor routines. */
#define _j64_init(t, x)		((j64_t){ .t = (x) })

#define j64_null()		_j64_init(w, J64_SUBTAG_LIT_NULL)
#define j64_false()		_j64_init(w, J64_SUBTAG_LIT_FALSE)
#define j64_true()		_j64_init(w, J64_SUBTAG_LIT_TRUE)
#define j64_bool(x)		_j64_init(w, (x) ? J64_TRUE : J64_FALSE)
#define j64_int(x)							\
    _j64_init(w, ((_j64_word_t)(x) << _J64_TAG_PRIM_INT_SIZE) | J64_TAG_PRIM_INT0)
j64_t	j64_float(double);
j64_t	j64_str(const char *);
#define j64_estr()		_j64_init(w, J64_SUBTAG_LIT_ESTR)
j64_t	j64_arr(j64_t *, size_t);
#define j64_earr()		_j64_init(w, J64_SUBTAG_LIT_EARR)
j64_t	j64_obj(j64_t *, size_t);
#define j64_eobj()		_j64_init(w, J64_SUBTAG_LIT_EOBJ)

/* Testing routines. */
#define	j64_is_lit(j)		(j64_prim_tag(j) == J64_TAG_PRIM_LIT)
#define j64_is_null(j)		(j64_subtag_lit(j) == J64_SUBTAG_LIT_NULL)

#define j64_is_float(j)		(j64_prim_tag(j) == J64_TAG_PRIM_FLOAT)

#define j64_is_false(j)		(j64_subtag_lit(j) == J64_SUBTAG_LIT_FALSE)
#define j64_is_true(j)		(j64_subtag_lit(j) == J64_SUBTAG_LIT_TRUE)
#define j64_is_bool(j)		(j64_is_false(j) || j64_is_true(j))

#define j64_is_int(j)							\
    (j64_prim_tag(j) == J64_TAG_PRIM_INT0 ||				\
     j64_prim_tag(j) == J64_TAG_PRIM_INT1)

#define j64_is_istr(j)		(j64_prim_tag(j) == J64_TAG_PRIM_ISTR)
#define j64_is_bstr(j)		(j64_prim_tag(j) == J64_TAG_PRIM_BSTR)
#define j64_is_estr(j)		(j64_subtag_lit(j) == J64_SUBTAG_LIT_ESTR)
#define j64_is_str(j)		(j64_is_bstr(j) || j64_is_istr(j) || j64_is_estr(j))

#define j64_is_barr(j)		(j64_prim_tag(j) == J64_TAG_PRIM_ARR)
#define j64_is_earr(j)		(j64_subtag_lit(j) == J64_SUBTAG_LIT_EARR)
#define j64_is_arr(j)		(j64_is_barr(j) || j64_is_earr(j))

#define j64_is_bobj(j)		(j64_prim_tag(j) == J64_TAG_PRIM_OBJ)
#define j64_is_eobj(j)		(j64_subtag_lit(j) == J64_SUBTAG_LIT_EOBJ)
#define j64_is_obj(j)		(j64_is_bobj(j) || j64_is_eobj(j))

/* Access routines (no type checking). */
#define j64_int_get(j)		((j).i >> _J64_TAG_PRIM_INT_SIZE)

#define j64_istr_get_at(j, i)		((j).b[7 - (i)])
#define j64_istr_set_at(j, i, x)	((j).b[7 - (i)] = (x))
#define j64_bstr_get_at(j, i)		((_j64_bstr_buf(j))[i])
#define j64_bstr_set_at(j, i, x)	((_j64_bstr_buf(j))[i] = (x))
#define j64_str_get_at(j, i)						\
    (j64_is_bstr(j) ? j64_bstr_get_at(j, i) : j64_istr_get_at(j, i))
#define j64_str_set_at(j, i, x)						\
    (j64_is_bstr(j) ? j64_bstr_set_at(j, i, x) : j64_istr_set_at(j, i, x))
#define _j64_istr_buf(j)	(&(j).b[1])
#define _j64_str_buf(j)							\
    (j64_is_bstr(j) ? _j64_bstr_buf(j) : (j64_is_istr(j) ? _j64_istr_buf(j) : NULL))
#define j64_str_len(j)							\
    (j64_is_bstr(j) ? j64_bstr_len(j) : (j64_is_istr(j) ? j64_istr_len(j) : 0))
size_t j64_str_get(j64_t, char *, size_t);

#define j64_arr_get_at(j, i)	(_j64_arr_buf(j)[i])

/* Debug */
void	j64_dbg(j64_t);

#endif
