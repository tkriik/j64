#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "j64.h"

/*
 * Constructor tests.
 */

int mk_null(void)
{
	return j64_is_null(j64_null());
}

int mk_false(void)
{
	return j64_is_false(j64_false());
}

int mk_true(void)
{
	return j64_is_true(j64_true());
}

int mk_empty_str(void)
{
	return j64_is_estr(j64_estr());
}

int mk_empty_arr(void)
{
	return j64_is_earr(j64_earr());
}

int mk_empty_obj(void)
{
	return j64_is_eobj(j64_eobj());
}

int mk_int0(void)
{
	return j64_is_int(j64_int(0));
}

int mk_int1(void)
{
	return j64_is_int(j64_int(-1));
}

int mk_float(void)
{
	return j64_is_float(j64_float(0.0));
}

int mk_str_with_len_0(void)
{
	return j64_is_estr(j64_str(""));
}

int mk_str_with_len_1(void)
{
	return j64_is_istr(j64_str("1"));
}

int mk_str_with_len_7(void)
{
	return j64_is_istr(j64_str("1234567"));
}

int mk_str_with_len_8(void)
{
	return j64_is_bstr(j64_str("12345678"));
}

/*
 * Equality tests.
 */

int int_eq_with_zero(void)
{
	return j64_int_get(j64_int(0)) == 0;
}

int int_eq_with_one_minus(void)
{
	return j64_int_get(j64_int(-1)) == -1;
}

int int_eq_with_one(void)
{
	return j64_int_get(j64_int(1)) == 1;
}

int int_eq_with_min(void)
{
	return j64_int_get(j64_int(J64_INT_MIN)) == J64_INT_MIN;
}

int int_eq_with_max(void)
{
	return j64_int_get(j64_int(J64_INT_MAX)) == J64_INT_MAX;
}

int int_neq_with_min_under(void)
{
	return j64_int_get(j64_int(J64_INT_MIN - 1)) != J64_INT_MIN - 1;
}

int int_neq_with_max_over(void)
{
	return j64_int_get(j64_int(J64_INT_MAX + 1)) != J64_INT_MAX + 1;
}

int str_len_eq_0(void)
{
	return j64_str_len(j64_str("")) == 0;
}

int str_len_eq_1(void)
{
	return j64_str_len(j64_str("1")) == 1;
}

int str_len_eq_7(void)
{
	return j64_str_len(j64_str("1234567")) == 7;
}

int str_len_eq_8(void)
{
	return j64_str_len(j64_str("12345678")) == 8;
}

int str_len_eq_16(void)
{
	return j64_str_len(j64_str("YELLOW SUBMARINE")) == 16;
}

#define STR_EQ_TEST_TEMPLATE(_s, _n)				\
	const char *s = _s;					\
	j64_t j = j64_str(_s);					\
	char b[_n + 1] = _s;					\
	size_t len = j64_str_get(j, b, sizeof(b));		\
	return len == _n && strcmp(s, b) == 0;

int str_eq_with_len_0(void)
{
	STR_EQ_TEST_TEMPLATE("", 0);
}

int str_eq_with_len_1(void)
{
	STR_EQ_TEST_TEMPLATE("1", 1);
}

int str_eq_with_len_7(void)
{
	STR_EQ_TEST_TEMPLATE("1234567", 7);
}

int str_eq_with_len_8(void)
{
	STR_EQ_TEST_TEMPLATE("12345678", 8);
}

int str_eq_with_len_16(void)
{
	STR_EQ_TEST_TEMPLATE("YELLOW SUBMARINE", 16);
}

struct test_info {
	const char *name;
	const char *descr;
	int (*fn)(void);
};

#define TEST(fn, descr) { #fn, descr, fn }

int run_test(struct test_info *i)
{
	int res = i->fn();

	if (res)
		fprintf(stderr, "%s: ok\n", i->name);
	else
		fprintf(stderr, "%s: FAIL (%s)\n", i->name, i->descr);

	return res;
}

struct test_info tests[] = {
	TEST(mk_null,			"tests null literal creation"),
	TEST(mk_false,			"tests false literal creation"),
	TEST(mk_true,			"tests true literal creation"),
	TEST(mk_empty_str,		"tests empty string literal creation"),
	TEST(mk_empty_arr,		"tests empty array literal creation"),
	TEST(mk_empty_obj,		"tests empty object literal creation"),
	TEST(mk_int0,			"tests zero integer creation"),
	TEST(mk_int1,			"tests complemented integer creation"),
	TEST(mk_float,			"tests floating-point number creation"),
	TEST(mk_str_with_len_0,		"tests string creation with empty string"),
	TEST(mk_str_with_len_1,		"tests string creation with one character"),
	TEST(mk_str_with_len_7,		"tests string creation with 7 characters"),
	TEST(mk_str_with_len_8,		"tests string creation with 8 characters"),

	TEST(uint_eq_with_zero,		"tests unsigned integer equality with zero"),
	TEST(uint_eq_with_one,		"tests unsigned integer equality with one"),
	TEST(uint_eq_with_max,		"tests unsigned integer equality with maximum value"),
	TEST(uint_neq_with_max_over,	"tests unsigned integer unequality with value over maximum"),
	TEST(int_eq_with_zero,		"tests signed integer equality with zero"),
	TEST(int_eq_with_one_minus,	"tests signed integer equality with minus one"),
	TEST(int_eq_with_one,		"tests signed integer equality with one"),
	TEST(int_eq_with_min,		"tests signed integer equality with minimum value"),
	TEST(int_eq_with_max,		"tests signed integer equality with maximum value"),
	TEST(int_neq_with_min_under,	"tests signed integer unequality with value under minimum"),
	TEST(int_neq_with_max_over,	"tests signed integer unequality with value over maximum"),
	TEST(str_len_eq_0,		"tests string length with empty string"),
	TEST(str_len_eq_1,		"tests string length with one character"),
	TEST(str_len_eq_7,		"tests string length with 7 characters"),
	TEST(str_len_eq_8,		"tests string length with 8 characters"),
	TEST(str_len_eq_16,		"tests string length with 16 characters"),
	TEST(str_eq_with_len_0,		"tests string equality with empty string"),
	TEST(str_eq_with_len_1,		"tests string equality with one character"),
	TEST(str_eq_with_len_7,		"tests string equality with 7 characters"),
	TEST(str_eq_with_len_8,		"tests string equality with 8 characters"),
	TEST(str_eq_with_len_16,	"tests string equality with 16 characters")
};

#define NTESTS (sizeof(tests) / sizeof(tests[0]))

int main(void)
{
	int nsuccess = 0, nfail = 0;
	for (size_t i = 0; i < NTESTS; i++) {
		int res = run_test(&tests[i]);
		if (res)
			nsuccess++;
		else
			nfail++;
	}

	fprintf(stderr,
	    "Success: %d\n"
	    "Fail:    %d\n",
	    nsuccess, nfail);

	if (nfail == 0)
		return EXIT_SUCCESS;
	else
		return EXIT_FAILURE;
}
