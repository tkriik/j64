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

int test_int_zero(void);
int test_int_pos_one(void);
int test_int_neg_one(void);
int test_int_max(void);
int test_int_min(void);
int test_int_overflow(void);
int test_int_underflow(void);

int test_int_zero_unsafe(void);
int test_int_pos_one_unsafe(void);
int test_int_neg_one_unsafe(void);
int test_int_max_unsafe(void);
int test_int_min_unsafe(void);
int test_int_overflow_unsafe(void);
int test_int_underflow_unsafe(void);

int test_istr_0(void);
int test_istr_1(void);
int test_istr_7(void);
int test_istr_8(void);

int test_istr_get_0(void);
int test_istr_get_1(void);
int test_istr_get_7(void);
int test_istr_get_8(void);

/* Test function and description list */
static const struct test TESTS[] = {
	TEST(test_undef,		"undefined literal construction"),
	TEST(test_null,			"null literal construction"),
	TEST(test_false,		"false literal construction"),
	TEST(test_true,			"true literal construction"),
	TEST(test_bool_false,		"boolean literal construction with false"),
	TEST(test_bool_true,		"boolean literal construction with true"),

	TEST(test_int_zero,		"zero integer construction"),
	TEST(test_int_pos_one,		"positive integer constructon"),
	TEST(test_int_neg_one,		"negative integer construction"),
	TEST(test_int_max,		"maximum integer construction"),
	TEST(test_int_min,		"minimum integer construction"),
	TEST(test_int_overflow,		"overflowed integer construction"),
	TEST(test_int_underflow,	"underflowed integer construction"),

	TEST(test_int_zero_unsafe,	"unsafe zero integer construction"),
	TEST(test_int_pos_one_unsafe,	"unsafe positive integer constructon"),
	TEST(test_int_neg_one_unsafe,	"unsafe negative integer construction"),
	TEST(test_int_max_unsafe,	"unsafe maximum integer construction"),
	TEST(test_int_min_unsafe,	"unsafe minimum integer construction"),
	TEST(test_int_overflow_unsafe,	"unsafe overflowed integer construction"),
	TEST(test_int_underflow_unsafe,	"unsafe underflowed integer construction"),

	TEST(test_istr_0,		"empty immediate string construction"),
	TEST(test_istr_1,		"immediate string construction with 1 character"),
	TEST(test_istr_7,		"immediate string construction with 7 characters"),
	TEST(test_istr_8,		"immediate string construction with 8 characters"),

	TEST(test_istr_get_0,		"empty immediate string storage"),
	TEST(test_istr_get_1,		"immediate string storage with 1 character"),
	TEST(test_istr_get_7,		"immediate string storage with 7 characters"),
	TEST(test_istr_get_8,		"immediate string storage with 8 characters")
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

#define MK_BOOL_TEST(TYPE, N)							\
int										\
test_bool_ ##TYPE(void)								\
{										\
	j64_t j = j64_bool(N);							\
	return j64_is_ ## TYPE(j);						\
}

MK_BOOL_TEST(false, 0)
MK_BOOL_TEST(true, 1)

#define MK_INT_TEST(NAME, X, Y, CONS, GET, TYPE)				\
int										\
test_int_ ## NAME(void)								\
{										\
	j64_t j = j64_ ## CONS(X);						\
	return j64_is_ ## TYPE(j) && j64_int_ ## GET(j) == Y;			\
}

MK_INT_TEST(zero, 0, 0, int, get, int)
MK_INT_TEST(pos_one, 1, 1, int, get, int)
MK_INT_TEST(neg_one, -1, -1, int, get, int)
MK_INT_TEST(max, J64_INT_MAX, J64_INT_MAX, int, get, int)
MK_INT_TEST(min, J64_INT_MIN, J64_INT_MIN, int, get, int)
MK_INT_TEST(overflow, J64_INT_MAX + 1, 0, int, get, undef)
MK_INT_TEST(underflow, J64_INT_MIN - 1, 0, int, get, undef)

MK_INT_TEST(zero_unsafe, 0, 0, int_unsafe, get_unsafe, int)
MK_INT_TEST(pos_one_unsafe, 1, 1, int_unsafe, get_unsafe, int)
MK_INT_TEST(neg_one_unsafe, -1, -1, int_unsafe, get_unsafe, int)
MK_INT_TEST(max_unsafe, J64_INT_MAX, J64_INT_MAX, int_unsafe, get_unsafe, int)
MK_INT_TEST(min_unsafe, J64_INT_MIN, J64_INT_MIN, int_unsafe, get_unsafe, int)
MK_INT_TEST(overflow_unsafe, INT64_MAX, -1, int_unsafe, get_unsafe, int)
MK_INT_TEST(underflow_unsafe, INT64_MIN, 0, int_unsafe, get_unsafe, int)

#define MK_ISTR_TEST(NAME, S, LENX, LENY)					\
int										\
test_istr_ ## NAME(void)							\
{										\
	j64_t j = j64_istr(S, LENX);						\
	return j64_is_istr(j) || j64_istr_len(j) == LENY;			\
}

MK_ISTR_TEST(0, "", 0, 0)
MK_ISTR_TEST(1, "1", 1, 1)
MK_ISTR_TEST(7, "1234567", 7, 7)
MK_ISTR_TEST(8, "12345678", 8, 7)

#define MK_ISTR_GET_TEST(NAME, S0, LEN0, S1, LEN1)				\
int										\
test_istr_get_ ## NAME(void)							\
{										\
	j64_t j;								\
	size_t len;								\
	uint8_t buf[LEN0 + 1];							\
	j = j64_istr(S0, LEN0);							\
	len = j64_istr_get(j, buf, sizeof(buf));				\
	return memcmp(S1, buf, len) == 0 && len == LEN1;			\
}

MK_ISTR_GET_TEST(0, "", 0, "", 0)
MK_ISTR_GET_TEST(1, "1", 1, "1", 1)
MK_ISTR_GET_TEST(7, "1234567", 7, "1234567", 7)
MK_ISTR_GET_TEST(8, "12345678", 8, "1234567", 7)
