#include <stdio.h>
#include <stdlib.h>

#include "j64.h"

/* Test function type */
typedef int (*test_fn)(void);

/* Test type */
struct test {
	const char	*name;
	const char	*descr;
	test_fn		 fn;
};

/* Test constructor */
#define TEST(fn, descr) { #fn, descr, fn }

int
run_test(const struct test *test)
{
	int success = test->fn();
	if (success) {
		/*fprintf(stderr, "ok: %s\n", test->name);*/
		return 1;
	} else {
		fprintf(stderr, "failed: %s (%s)\n", test->name, test->descr);
		return 0;
	}
}

int
run_tests(const struct test *tests, int ntests)
{
	int i;
	int success;
	int nsuccess = 0, nfail = 0, ntotal = 0;

	for (i = 0; i < ntests; i++) {
		success = run_test(&tests[i]);
		if (success)
			nsuccess++;
		else
			nfail++;
		ntotal++;
	}

	if (nfail == 0) {
		fprintf(stderr, "All %d tests passed!\n", nsuccess);
		return 1;
	} else {
		fprintf(stderr, "%d out of %d tests failed.\n", nfail, ntotal);
		return 0;
	}
}

/* Test declarations */
int test_undef(void);
int test_null(void);
int test_false(void);
int test_true(void);
int test_bool_false(void);
int test_bool_true(void);
int test_estr(void);
int test_earr(void);
int test_eobj(void);

int test_null_encode_4(void);
int test_null_encode_2(void);
int test_false_encode_5(void);
int test_false_encode_2(void);
int test_true_encode_4(void);
int test_true_encode_2(void);
int test_estr_encode_2(void);
int test_estr_encode_1(void);
int test_earr_encode_2(void);
int test_earr_encode_1(void);
int test_eobj_encode_2(void);
int test_eobj_encode_1(void);

int test_int_zero(void);
int test_int_one(void);
int test_int_minus_one(void);
int test_int_max(void);
int test_int_min(void);
int test_int_overflow(void);
int test_int_get_zero(void);
int test_int_get_one(void);
int test_int_get_minus_one(void);
int test_int_get_max(void);
int test_int_get_min(void);
int test_int_get_overflow(void);

int test_float_zero(void);
int test_float_one(void);
int test_float_neg_zero(void);
int test_float_neg_one(void);
int test_float_small(void);
int test_float_too_small(void);
int test_float_get_zero(void);
int test_float_get_one(void);
int test_float_get_neg_zero(void);
int test_float_get_neg_one(void);
int test_float_get_small(void);
int test_float_get_too_small(void);

int test_istr_0(void);
int test_istr_1(void);
int test_istr_7(void);
int test_istr_8(void);
int test_istr_len_0(void);
int test_istr_len_1(void);
int test_istr_len_7(void);
int test_istr_len_8(void);
int test_istr_get_0(void);
int test_istr_get_1(void);
int test_istr_get_7(void);
int test_istr_get_8(void);

int test_bstr_0(void);
int test_bstr_1(void);
int test_bstr_8(void);
int test_bstr_65536(void);
int test_bstr_len_0(void);
int test_bstr_len_1(void);
int test_bstr_len_8(void);
int test_bstr_len_65536(void);
int test_bstr_get_0(void);
int test_bstr_get_1(void);
int test_bstr_get_8(void);
int test_bstr_get_65536(void);

int test_barr_alloc_0(void);
int test_barr_alloc_1(void);
int test_barr_alloc_8(void);
int test_barr_alloc_65536(void);
int test_barr_alloc_overflow(void);
int test_barr_alloc_cap_0(void);
int test_barr_alloc_cap_1(void);
int test_barr_alloc_cap_8(void);
int test_barr_alloc_cap_65536(void);
int test_barr_realloc_0_0(void);
int test_barr_realloc_0_1(void);
int test_barr_realloc_0_65536(void);
int test_barr_realloc_1_0(void);
int test_barr_realloc_1_1(void);
int test_barr_realloc_1_65536(void);
int test_barr_realloc_65536_0(void);
int test_barr_realloc_65536_1(void);
int test_barr_realloc_65536_65536(void);
int test_barr_realloc_overflow(void);
int test_barr_set_get_1(void);
int test_barr_set_get_8(void);
int test_barr_set_get_65536(void);
int test_barr_set_free_get_1(void);
int test_barr_set_free_get_8(void);
int test_barr_set_free_get_65536(void);

/* Test function and description list */
static const struct test TESTS[] = {
	TEST(test_undef,			"undefined literal construction"),
	TEST(test_null,				"null literal construction"),
	TEST(test_false,			"false literal construction"),
	TEST(test_true,				"true literal construction"),
	TEST(test_bool_false,			"boolean literal construction with false"),
	TEST(test_bool_true,			"boolean literal construction with true"),
	TEST(test_estr,				"empty string literal construction"),
	TEST(test_earr,				"empty array literal construction"),
	TEST(test_eobj,				"empty object literal construction"),

	TEST(test_null_encode_4,		"null literal encoding with 4 bytes"),
	TEST(test_null_encode_2,		"null literal encoding with 2 bytes"),
	TEST(test_false_encode_5,		"false literal encoding with 5 bytes"),
	TEST(test_false_encode_2,		"false literal encoding with 2 bytes"),
	TEST(test_true_encode_4,		"true literal encoding with 4 bytes"),
	TEST(test_true_encode_2,		"true literal encoding with 2 bytes"),
	TEST(test_estr_encode_2,		"empty string encoding with 2 bytes"),
	TEST(test_estr_encode_1,		"empty string encoding with 1 byte"),
	TEST(test_earr_encode_2,		"empty array encoding with 2 bytes"),
	TEST(test_earr_encode_1,		"empty array encoding with 1 byte"),
	TEST(test_eobj_encode_2,		"empty object encoding with 2 bytes"),
	TEST(test_eobj_encode_1,		"empty object encoding with 1 byte"),

	TEST(test_int_zero,			"zero integer construction"),
	TEST(test_int_one,			"positive integer constructon"),
	TEST(test_int_minus_one,		"negative integer construction"),
	TEST(test_int_max,			"maximum integer construction"),
	TEST(test_int_min,			"minimum integer construction"),
	TEST(test_int_overflow,			"overflowed integer construction"),
	TEST(test_int_get_zero,			"zero integer storage"),
	TEST(test_int_get_one,			"positive integer storage"),
	TEST(test_int_get_minus_one,		"negative integer storage"),
	TEST(test_int_get_max,			"maximum integer storage"),
	TEST(test_int_get_min,			"minimum integer storage"),
	TEST(test_int_get_overflow,		"overflowed integer storage"),

	TEST(test_float_zero,			"zero floating-point construction"),
	TEST(test_float_one,			"positive floating-point construction"),
	TEST(test_float_neg_one,		"negative floating-point construction"),
	TEST(test_float_neg_zero,		"negative zero floating-point construction"),
	TEST(test_float_small,			"very small floating-point construction"),
	TEST(test_float_too_small,		"too small floating-point construction"),
	TEST(test_float_get_zero,		"zero floating-point storage"),
	TEST(test_float_get_one,		"positive floating-point storage"),
	TEST(test_float_get_neg_one,		"negative floating-point storage"),
	TEST(test_float_get_neg_zero,		"negative zero floating-point storage"),
	TEST(test_float_get_small,		"very small floating-point storage"),
	TEST(test_float_get_too_small,		"too small floating-point storage"),

	TEST(test_istr_0,			"empty immediate string construction"),
	TEST(test_istr_1,			"immediate string construction with 1 character"),
	TEST(test_istr_7,			"immediate string construction with 7 characters"),
	TEST(test_istr_8,			"immediate string construction with 8 characters"),
	TEST(test_istr_len_0,			"empty immediate string length"),
	TEST(test_istr_len_1,			"immediate string length with 1 character"),
	TEST(test_istr_len_7,			"immediate string length with 7 characters"),
	TEST(test_istr_len_8,			"immediate string length with 8 characters"),
	TEST(test_istr_get_0,			"empty immediate string storage"),
	TEST(test_istr_get_1,			"immediate string storage with 1 character"),
	TEST(test_istr_get_7,			"immediate string storage with 7 characters"),
	TEST(test_istr_get_8,			"immediate string storage with 8 characters"),

	TEST(test_bstr_0,			"empty boxed string construction"),
	TEST(test_bstr_1,			"boxed string construction with 1 character"),
	TEST(test_bstr_1,			"boxed string construction with 8 character"),
	TEST(test_bstr_65536,			"boxed string construction with 65536 characters"),
	TEST(test_bstr_len_0,			"empty boxed string length"),
	TEST(test_bstr_len_1,			"boxed string length with 1 character"),
	TEST(test_bstr_len_1,			"boxed string length with 8 character"),
	TEST(test_bstr_len_65536,		"boxed string length with 65536 characters"),
	TEST(test_bstr_get_0,			"empty boxed string storage"),
	TEST(test_bstr_get_1,			"boxed string storage with 1 character"),
	TEST(test_bstr_get_1,			"boxed string storage with 8 character"),
	TEST(test_bstr_get_65536,		"boxed string storage with 65536 characters"),

	TEST(test_barr_alloc_0,			"empty boxed array construction"),
	TEST(test_barr_alloc_1,			"boxed array construction of capacity 1"),
	TEST(test_barr_alloc_8,			"boxed array construction of capacity 8"),
	TEST(test_barr_alloc_65536,		"boxed array construction of capacity 65536"),
	TEST(test_barr_alloc_overflow,		"boxed array construction with overflowed capacity"),
	TEST(test_barr_alloc_cap_0,		"empty boxed array capacity"),
	TEST(test_barr_alloc_cap_1,		"boxed array capacity with capacity 1"),
	TEST(test_barr_alloc_cap_8,		"boxed array capacity with capacity 8"),
	TEST(test_barr_alloc_cap_65536,		"boxed array capacity with capacity 65536"),
	TEST(test_barr_realloc_0_0,		"boxed array reallocation from 0 to 0"),
	TEST(test_barr_realloc_0_1,		"boxed array reallocation from 0 to 1"),
	TEST(test_barr_realloc_0_65536,		"boxed array reallocation from 0 to 65536"),
	TEST(test_barr_realloc_1_0,		"boxed array reallocation from 1 to 0"),
	TEST(test_barr_realloc_1_1,		"boxed array reallocation from 1 to 1"),
	TEST(test_barr_realloc_1_65536,		"boxed array reallocation from 1 to 65536"),
	TEST(test_barr_realloc_65536_0,		"boxed array reallocation from 65536 to 0"),
	TEST(test_barr_realloc_65536_1,		"boxed array reallocation from 65536 to 1"),
	TEST(test_barr_realloc_65536_65536,	"boxed array reallocation from 65536 to 65536"),
	TEST(test_barr_realloc_overflow,	"boxed array reallocation with overflowed capacity"),
	TEST(test_barr_set_get_1,		"boxed array element storage with 1 element"),
	TEST(test_barr_set_get_8,		"boxed array element storage with 8 elements"),
	TEST(test_barr_set_get_65536,		"boxed array element storage with 65536 elements"),
	TEST(test_barr_set_free_get_1,		"boxed array freed element storage with 1 element"),
	TEST(test_barr_set_free_get_8,		"boxed array freed element storage with 8 elements"),
	TEST(test_barr_set_free_get_65536,	"boxed array freed element storage with 65536 elements")
};

#define NTESTS (sizeof(TESTS) / sizeof(TESTS[0]))

int
main(void)
{
	int success;
	
	success = run_tests(TESTS, NTESTS);
	if (success)
		return EXIT_SUCCESS;
	else
		return EXIT_FAILURE;
}

/*
 * Test generators and definitions
 */

#define MK_LIT_TEST(TYPE)							\
int										\
test_ ## TYPE(void)								\
{										\
	j64_t j = j64_ ## TYPE();						\
	return j64_is_ ## TYPE(j);						\
}

MK_LIT_TEST(undef)
MK_LIT_TEST(null)
MK_LIT_TEST(false)
MK_LIT_TEST(true)
MK_LIT_TEST(estr)
MK_LIT_TEST(earr)
MK_LIT_TEST(eobj)

#define MK_BOOL_TEST(TYPE, N)							\
int										\
test_bool_ ##TYPE(void)								\
{										\
	j64_t j = j64_bool(N);							\
	return j64_is_ ## TYPE(j);						\
}

MK_BOOL_TEST(false, 0)
MK_BOOL_TEST(true, 1)

#define MK_LIT_ENCODE_TEST(TYPE, LEN, S)					\
int										\
test_ ## TYPE ## _encode_ ## LEN(void)						\
{                                                                               \
	char buf[LEN + 1];                                                      \
	memset(buf, '\0', sizeof(buf));                                         \
	j64_ ## TYPE ## _encode(buf, LEN);                                      \
	return strcmp(buf, S) == 0;                                             \
}

MK_LIT_ENCODE_TEST(null, 4, "null")
MK_LIT_ENCODE_TEST(null, 2, "nu")
MK_LIT_ENCODE_TEST(false, 5, "false")
MK_LIT_ENCODE_TEST(false, 2, "fa")
MK_LIT_ENCODE_TEST(true, 4, "true")
MK_LIT_ENCODE_TEST(true, 2, "tr")
MK_LIT_ENCODE_TEST(estr, 2, "\"\"")
MK_LIT_ENCODE_TEST(estr, 1, "\"")
MK_LIT_ENCODE_TEST(earr, 2, "[]")
MK_LIT_ENCODE_TEST(earr, 1, "[")
MK_LIT_ENCODE_TEST(eobj, 2, "{}")
MK_LIT_ENCODE_TEST(eobj, 1, "{")

#define MK_INT_TEST(NAME, X)							\
int										\
test_int_ ## NAME(void)								\
{										\
	j64_t j = j64_int(X);							\
	return j64_is_int(j);							\
}

MK_INT_TEST(zero, 0)
MK_INT_TEST(one, 1)
MK_INT_TEST(minus_one, -1)
MK_INT_TEST(max, J64_INT_MAX)
MK_INT_TEST(min, J64_INT_MIN)
MK_INT_TEST(overflow, J64_INT_MAX + 1)

#define MK_INT_GET_TEST(NAME, X, Y)						\
int										\
test_int_get_ ## NAME(void)							\
{										\
	j64_t j = j64_int(X);							\
	return j64_int_get(j) == Y;						\
}

MK_INT_GET_TEST(zero, 0, 0)
MK_INT_GET_TEST(one, 1, 1)
MK_INT_GET_TEST(minus_one, -1, -1)
MK_INT_GET_TEST(max, J64_INT_MAX, J64_INT_MAX)
MK_INT_GET_TEST(min, J64_INT_MIN, J64_INT_MIN)
MK_INT_GET_TEST(overflow, J64_INT_MAX + 1, J64_INT_MIN)

#define MK_FLOAT_TEST(NAME, X)							\
int										\
test_float_ ## NAME(void)							\
{										\
	j64_t j = j64_float(X);                                                 \
	return j64_is_float(j);                                                 \
}

#define MK_FLOAT_TEST_W(NAME, X)						\
int										\
test_float_ ## NAME(void)							\
{										\
	j64_t j;								\
	union {									\
		uint64_t w;							\
		double f;							\
	} u;									\
	u.w = X;								\
	j = j64_float(u.f);							\
	return j64_is_float(j);							\
}

/* TODO: NaN, negative inf and positive inf */
MK_FLOAT_TEST(zero, 0.0)
MK_FLOAT_TEST(neg_zero, -0.0)
MK_FLOAT_TEST(one, 1.0)
MK_FLOAT_TEST(neg_one, 1.0)
MK_FLOAT_TEST_W(small, 0x8000000000000008ULL)
MK_FLOAT_TEST_W(too_small, 0x8000000000000004ULL)

#define MK_FLOAT_GET_TEST(NAME, X)						\
int										\
test_float_get_ ## NAME(void)							\
{										\
	j64_t j = j64_float(X);							\
	return j64_float_get(j) == X;						\
}

#define MK_FLOAT_GET_TEST_W(NAME, X, Y)						\
int										\
test_float_get_ ## NAME(void)							\
{										\
	j64_t j;								\
	union {									\
		uint64_t w;							\
		double f;							\
	} u;									\
	u.w = X;								\
	j = j64_float(u.f);							\
	u.f = j64_float_get(j);							\
	return u.w == Y;							\
}

/* TODO: NaN, negative inf and positive inf */
MK_FLOAT_GET_TEST(zero, 0.0)
MK_FLOAT_GET_TEST(one, 1.0)
MK_FLOAT_GET_TEST(neg_zero, -0.0)
MK_FLOAT_GET_TEST(neg_one, -1.0)
MK_FLOAT_GET_TEST_W(small, 0x8000000000000008ULL, 0x8000000000000008ULL)
MK_FLOAT_GET_TEST_W(too_small, 0x800000000000000cULL, 0x8000000000000008ULL)

#define MK_ISTR_TEST(S, LEN)							\
int										\
test_istr_ ## LEN(void)								\
{										\
	j64_t j = j64_istr(S, LEN);						\
	return j64_is_istr(j);							\
}

MK_ISTR_TEST("", 0)
MK_ISTR_TEST("1", 1)
MK_ISTR_TEST("1234567", 7)
MK_ISTR_TEST("12345678", 8)

#define MK_ISTR_LEN_TEST(S, LEN0, LEN1)						\
int										\
test_istr_len_ ## LEN0(void)                                                    \
{                                                                               \
	j64_t j = j64_istr(S, LEN0);                                            \
	return j64_istr_len(j) == LEN1;                                         \
}

MK_ISTR_LEN_TEST("", 0, 0)
MK_ISTR_LEN_TEST("1", 1, 1)
MK_ISTR_LEN_TEST("1234567", 7, 7)
MK_ISTR_LEN_TEST("12345678", 8, 7)

#define MK_ISTR_GET_TEST(S0, LEN0, S1, LEN1)					\
int										\
test_istr_get_ ## LEN0(void)							\
{										\
	j64_t j;                                                                \
	size_t n;								\
	uint8_t buf[LEN0 + 1];                                                  \
	memset(buf, '\0', sizeof(buf));                                         \
	j = j64_istr(S0, LEN0);                                                 \
	n = j64_istr_get(j, buf, LEN0);						\
	return memcmp(S1, buf, n) == 0 && n == LEN1;				\
}

MK_ISTR_GET_TEST("", 0, "", 0)
MK_ISTR_GET_TEST("1", 1, "1", 1)
MK_ISTR_GET_TEST("1234567", 7, "1234567", 7)
MK_ISTR_GET_TEST("12345678", 8, "1234567", 7)

#define MK_BSTR_TEST(LEN)							\
int										\
test_bstr_ ## LEN(void)								\
{										\
	int res;								\
	j64_t j;								\
	uint8_t buf[LEN + 1];							\
	memset(buf, '\0', sizeof(buf));						\
	j = j64_bstr(buf, LEN);							\
	res = j64_is_bstr(j);							\
	j64_bstr_free(j);							\
	return res;								\
}

MK_BSTR_TEST(0)
MK_BSTR_TEST(1)
MK_BSTR_TEST(8)
MK_BSTR_TEST(65536)

#define MK_BSTR_LEN_TEST(LEN)							\
int										\
test_bstr_len_ ## LEN(void)							\
{										\
	int res;								\
	j64_t j;                                                                \
	uint8_t buf[LEN + 1];                                                   \
	memset(buf, '\0', LEN);							\
	j = j64_bstr(buf, LEN);							\
	res = j64_bstr_len(j) == LEN;						\
	j64_bstr_free(j);							\
	return res;								\
}

MK_BSTR_LEN_TEST(0)
MK_BSTR_LEN_TEST(1)
MK_BSTR_LEN_TEST(8)
MK_BSTR_LEN_TEST(65536)

#define MK_BSTR_GET_TEST(LEN)							\
int										\
test_bstr_get_ ## LEN(void)							\
{										\
	int res;								\
	j64_t j;								\
	size_t n;                                                               \
	uint8_t src[LEN + 1];                                                   \
	uint8_t dst[LEN + 1];                                                   \
	memset(src, 0xFE, LEN);							\
	j = j64_bstr(src, LEN);                                         	\
	n = j64_bstr_get(j, dst, LEN);                                  	\
	res = memcmp(src, dst, LEN) == 0 && n == LEN;				\
	j64_bstr_free(j);							\
	return res;								\
}

MK_BSTR_GET_TEST(0)
MK_BSTR_GET_TEST(1)
MK_BSTR_GET_TEST(8)
MK_BSTR_GET_TEST(65536)

#define MK_BARR_ALLOC_TEST(CAP)							\
int										\
test_barr_alloc_ ## CAP(void)							\
{										\
	int res;								\
	j64_t j = j64_barr_alloc(CAP);                                          \
	res = j64_is_barr(j);							\
	j64_barr_free(j);							\
	return res;								\
}

MK_BARR_ALLOC_TEST(0)
MK_BARR_ALLOC_TEST(1)
MK_BARR_ALLOC_TEST(8)
MK_BARR_ALLOC_TEST(65536)

int
test_barr_alloc_overflow(void)
{
	int res;
	j64_t j = j64_barr_alloc(J64_BARR_CAP_MAX + 1);
	res = j64_is_undef(j);
	j64_free(j);
	return res;
}

#define MK_BARR_ALLOC_CAP_TEST(CAP)						\
int										\
test_barr_alloc_cap_ ## CAP(void)						\
{										\
	int res;								\
	j64_t j = j64_barr_alloc(CAP);						\
	res = j64_barr_cap(j) == CAP;						\
	j64_barr_free(j);							\
	return res;								\
}

MK_BARR_ALLOC_CAP_TEST(0)
MK_BARR_ALLOC_CAP_TEST(1)
MK_BARR_ALLOC_CAP_TEST(8)
MK_BARR_ALLOC_CAP_TEST(65536)

#define MK_BARR_REALLOC_TEST(CAP, NEW_CAP)					\
int										\
test_barr_realloc_ ## CAP ## _ ## NEW_CAP(void)					\
{                                                                               \
	int res;                                                                \
	j64_t j = j64_barr_alloc(CAP);                                          \
	j64_barr_realloc(&j, NEW_CAP);						\
	res = j64_is_barr(j);                                                   \
	j64_barr_free(j);                                                       \
	return res;                                                             \
}

MK_BARR_REALLOC_TEST(0, 0)
MK_BARR_REALLOC_TEST(0, 1)
MK_BARR_REALLOC_TEST(0, 65536)
MK_BARR_REALLOC_TEST(1, 0)
MK_BARR_REALLOC_TEST(1, 1)
MK_BARR_REALLOC_TEST(1, 65536)
MK_BARR_REALLOC_TEST(65536, 0)
MK_BARR_REALLOC_TEST(65536, 1)
MK_BARR_REALLOC_TEST(65536, 65536)

int
test_barr_realloc_overflow(void)
{
	int res;
	j64_t j;
	j = j64_barr_alloc(0);
	res = j64_barr_realloc(&j, J64_BARR_CAP_MAX + 1);
	j64_barr_free(j);
	return !res;
}

#define MK_BARR_SET_GET_TEST(CAP, SET)						\
int										\
test_barr_ ## SET ## _get_ ## CAP(void)						\
{										\
	int res = 1;								\
	size_t i;								\
	j64_t k;								\
	j64_t j = j64_barr_alloc(CAP);						\
	for (i = 0; i < CAP; i++)						\
		j64_barr_ ## SET(j, j64_int((int64_t)i), i);			\
	for (i = 0; i < CAP; i++) {						\
		k = j64_barr_get(j, i);						\
		if (!j64_is_int(k) || j64_int_get(k) != (int64_t)i) {		\
			res = 0;                                                \
			break;                                                  \
		}                                                               \
	}									\
	j64_barr_free(j);							\
	return res;								\
}

MK_BARR_SET_GET_TEST(1, set)
MK_BARR_SET_GET_TEST(8, set)
MK_BARR_SET_GET_TEST(65536, set)
MK_BARR_SET_GET_TEST(1, set_free)
MK_BARR_SET_GET_TEST(8, set_free)
MK_BARR_SET_GET_TEST(65536, set_free)
