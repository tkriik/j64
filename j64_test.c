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

/* Test function and description list */
static const struct test TESTS[] = {
	TEST(test_undef,		"undefined literal construction"),
	TEST(test_null,			"null literal construction"),
	TEST(test_false,		"false literal construction"),
	TEST(test_true,			"true literal construction"),
	TEST(test_bool_false,		"boolean literal construction with false"),
	TEST(test_bool_true,		"boolean literal construction with true"),
	TEST(test_estr,			"empty string literal construction"),

	TEST(test_int_zero,		"zero integer construction"),
	TEST(test_int_one,		"positive integer constructon"),
	TEST(test_int_minus_one,	"negative integer construction"),
	TEST(test_int_max,		"maximum integer construction"),
	TEST(test_int_min,		"minimum integer construction"),
	TEST(test_int_overflow,		"overflowed integer construction"),
	TEST(test_int_get_zero,		"zero integer storage"),
	TEST(test_int_get_one,		"positive integer storage"),
	TEST(test_int_get_minus_one,	"negative integer storage"),
	TEST(test_int_get_max,		"maximum integer storage"),
	TEST(test_int_get_min,		"minimum integer storage"),
	TEST(test_int_get_overflow,	"overflowed integer storage"),

	TEST(test_istr_0,		"empty immediate string construction"),
	TEST(test_istr_1,		"immediate string construction with 1 character"),
	TEST(test_istr_7,		"immediate string construction with 7 characters"),
	TEST(test_istr_8,		"immediate string construction with 8 characters"),
	TEST(test_istr_len_0,		"empty immediate string length"),
	TEST(test_istr_len_1,		"immediate string length with 1 character"),
	TEST(test_istr_len_7,		"immediate string length with 7 characters"),
	TEST(test_istr_len_8,		"immediate string length with 8 characters"),
	TEST(test_istr_get_0,		"empty immediate string storage"),
	TEST(test_istr_get_1,		"immediate string storage with 1 character"),
	TEST(test_istr_get_7,		"immediate string storage with 7 characters"),
	TEST(test_istr_get_8,		"immediate string storage with 8 characters"),

	TEST(test_bstr_0,		"empty boxed string construction"),
	TEST(test_bstr_1,		"boxed string construction with 1 character"),
	TEST(test_bstr_1,		"boxed string construction with 8 character"),
	TEST(test_bstr_65536,		"boxed string construction with 65536 characters")
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

#define MK_BOOL_TEST(TYPE, N)							\
int										\
test_bool_ ##TYPE(void)								\
{										\
	j64_t j = j64_bool(N);							\
	return j64_is_ ## TYPE(j);						\
}

MK_BOOL_TEST(false, 0)
MK_BOOL_TEST(true, 1)

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
	size_t ncopy;                                                           \
	uint8_t buf[LEN0 + 1];                                                  \
	memset(buf, '\0', sizeof(buf));                                         \
	j = j64_istr(S0, LEN0);                                                 \
	ncopy = j64_istr_get(j, buf, LEN0);                                     \
	return memcmp(S1, buf, ncopy) == 0 && ncopy == LEN1;                    \
}

MK_ISTR_GET_TEST("", 0, "", 0)
MK_ISTR_GET_TEST("1", 1, "1", 1)
MK_ISTR_GET_TEST("1234567", 7, "1234567", 7)
MK_ISTR_GET_TEST("12345678", 8, "1234567", 7)

#define MK_BSTR_TEST(LEN)							\
int										\
test_bstr_ ## LEN(void)								\
{										\
	j64_t j;								\
	size_t i;								\
	uint8_t buf[LEN + 1];							\
	for (i = 0; i < LEN; i++)						\
		buf[i] = (uint8_t)i;						\
	j = j64_bstr(buf, LEN);							\
	return j64_is_bstr(j) && j64_bstr_len(j) == LEN;			\
} /* TODO: free */

MK_BSTR_TEST(0)
MK_BSTR_TEST(1)
MK_BSTR_TEST(8)
MK_BSTR_TEST(65536)
