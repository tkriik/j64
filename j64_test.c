#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "j64.h"

/*
 * System tests.
 */

int sys_word_size(void)
{
	return sizeof(_j64_word_t) == 8;
}

int sys_ptr_size(void)
{
	return sizeof(_j64_ptr_t) <= sizeof(_j64_word_t);
}

int sys_j64_union_size(void)
{
	return sizeof(j64_t) == sizeof(_j64_word_t);
}

#define NMALLOCS	65536
#define MALLOC_SIZE	16

int sys_malloc_alignment(void)
{
	int res = 1;

	union {
		_j64_ptr_t  p;
		_j64_word_t w;
	} us[NMALLOCS];

	size_t nmallocs = 0;
	for (size_t i = 0; i < NMALLOCS; i++) {
		us[i].p = malloc(MALLOC_SIZE);
		if (us[i].p == NULL) {
			res = 0;
			goto free;
		}
		if (us[i].w != (us[i].w & ~_J64_TAG_PRIM_MASK)) {
			res = 0;
			goto free;
		}
		nmallocs++;
	}

free:
	for (size_t i = 0; i < nmallocs; i++)
		free(us[i].p);

	return res;
}

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

int mk_str_with_0(void)
{
	return j64_is_estr(j64_str(""));
}

int mk_str_with_1(void)
{
	return j64_is_istr(j64_str("1"));
}

int mk_str_with_7(void)
{
	return j64_is_istr(j64_str("1234567"));
}

int mk_str_with_8(void)
{
	return j64_is_bstr(j64_str("12345678"));
}

int mk_str_with_16(void)
{
	return j64_is_bstr(j64_str("YELLOW SUBMARINE"));
}

int mk_arr_with_0(void)
{
	return j64_is_earr(j64_arr(NULL, 0));
}

#define MK_ARR_TEST_TEMPLATE(_n)					\
	int res = 0;							\
	j64_t js[_n];							\
	for (size_t i = 0; i < _n; i++)					\
		js[i] = j64_null();					\
	j64_t j = j64_arr(js, _n);					\
	res = j64_is_barr(j);						\
	j64_free(j);							\
	return res

int mk_arr_with_1(void)
{
	MK_ARR_TEST_TEMPLATE(1);
}

int mk_arr_with_8(void)
{
	MK_ARR_TEST_TEMPLATE(8);
}

int mk_arr_with_64(void)
{
	MK_ARR_TEST_TEMPLATE(64);
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

int str_len_with_1(void)
{
	return j64_istr_len(j64_str("1")) == 1;
}

int str_len_with_7(void)
{
	return j64_istr_len(j64_str("1234567")) == 7;
}

int str_len_with_8(void)
{
	return j64_bstr_len(j64_str("12345678")) == 8;
}

int str_len_with_16(void)
{
	return j64_bstr_len(j64_str("YELLOW SUBMARINE")) == 16;
}

#define STR_IDX_GET_TEST_TEMPLATE(_s, _t)				\
	int res = 1;							\
	const char *s = _s;						\
	j64_t j = j64_str(_s);						\
	for (size_t i = 0; i < j64_ ## _t ## _len(j); i++) {		\
		if (j64_ ## _t ##_get_at(j, i) != s[i]) {		\
			res = 0;					\
			break;						\
		}							\
	}								\
	j64_free(j);							\
	return res

int str_idx_get_with_1(void)
{
	STR_IDX_GET_TEST_TEMPLATE("1", istr);
}

int str_idx_get_with_7(void)
{
	STR_IDX_GET_TEST_TEMPLATE("1234567", istr);
}

int str_idx_get_with_8(void)
{
	STR_IDX_GET_TEST_TEMPLATE("12345678", bstr);
}

int str_idx_get_with_16(void)
{
	STR_IDX_GET_TEST_TEMPLATE("YELLOW SUBMARINE", bstr);
}

#define STR_IDX_SET_TEST_TEMPLATE(_s, _d, _t)				\
	int res = 1;							\
	const char *d = _s;						\
	j64_t j = j64_str(_s);						\
	for (size_t i = 0; i < j64_ ## _t ## _len(j); i++)		\
		j64_ ## _t ## _set_at(j, i, d[i]);			\
	for (size_t i = 0; i < j64_ ## _t ## _len(j); i++) {		\
		if (j64_ ## _t ## _get_at(j, i) != d[i]) {		\
			res = 0;					\
			break;						\
		}							\
	}								\
	j64_free(j);							\
	return res


int str_idx_set_with_1(void)
{
	STR_IDX_SET_TEST_TEMPLATE("1", "A", istr);
}

int str_idx_set_with_7(void)
{
	STR_IDX_SET_TEST_TEMPLATE("1234567", "ABCDEFG", istr);
}

int str_idx_set_with_8(void)
{
	STR_IDX_SET_TEST_TEMPLATE("12345678", "ABCDEFGH", bstr);
}

int str_idx_set_with_16(void)
{
	STR_IDX_SET_TEST_TEMPLATE("YELLOW SUBMARINE", "0123456789abcdef", bstr);
}

#define STR_EQ_TEST_TEMPLATE(_s, _n, _t)				\
	int res = 0;							\
	const char *s = _s;						\
	j64_t j = j64_str(_s);						\
	char b[_n + 1] = _s;						\
	size_t len = j64_ ##_t ## _get(j, b, sizeof(b));		\
	res = len == _n && strcmp(s, b) == 0;				\
	j64_free(j);							\
	return res

int str_eq_with_1(void)
{
	STR_EQ_TEST_TEMPLATE("1", 1, istr);
}

int str_eq_with_7(void)
{
	STR_EQ_TEST_TEMPLATE("1234567", 7, istr);
}

int str_eq_with_8(void)
{
	STR_EQ_TEST_TEMPLATE("12345678", 8, bstr);
}

int str_eq_with_16(void)
{
	STR_EQ_TEST_TEMPLATE("YELLOW SUBMARINE", 16, bstr);
}

#define ARR_LEN_TEST_TEMPLATE(_n)					\
	int res = 0;							\
	j64_t js[_n];							\
	for (size_t i = 0; i < _n; i++)					\
		js[i] = j64_null();					\
	j64_t j = j64_arr(js, _n);					\
	res = j64_arr_len(j) == _n;					\
	j64_free(j);							\
	return res

int arr_len_with_1(void)
{
	ARR_LEN_TEST_TEMPLATE(1);
}

int arr_len_with_8(void)
{
	ARR_LEN_TEST_TEMPLATE(8);
}

int arr_len_with_64(void)
{
	ARR_LEN_TEST_TEMPLATE(64);
}

#define ARR_IDX_GET_TEMPLATE(_n)					\
	int res = 1;							\
	j64_t js[_n];							\
	for (size_t i = 0; i < _n; i++)					\
		js[i] = j64_int(i);					\
	j64_t j = j64_arr(js, _n);					\
	for (size_t i = 0; i < _n; i++)					\
		if (!(j64_eq(j64_arr_get_at(j, i), j64_int(i)))) {	\
			res = 0;					\
			break;						\
		}							\
	j64_free(j);							\
	return res

int arr_idx_get_with_1(void)
{
	ARR_IDX_GET_TEMPLATE(1);
}

int arr_idx_get_with_8(void)
{
	ARR_IDX_GET_TEMPLATE(8);
}

int arr_idx_get_with_64(void)
{
	ARR_IDX_GET_TEMPLATE(64);
}

#define ARR_IDX_SET_TEMPLATE(_n)					\
	int res = 1;							\
	j64_t js[_n];							\
	for (size_t i = 0; i < _n; i++)					\
		js[i] = j64_null();					\
	j64_t j = j64_arr(js, _n);					\
	for (size_t i = 0; i < _n; i++) {				\
		j64_arr_set_at(j, i, j64_int(i));			\
		if (!(j64_eq(j64_arr_get_at(j, i), j64_int(i)))) {	\
			res = 0;					\
			break;						\
		}							\
	}								\
	j64_free(j);							\
	return res

int arr_idx_set_with_1(void)
{
	ARR_IDX_SET_TEMPLATE(1);
}

int arr_idx_set_with_8(void)
{
	ARR_IDX_SET_TEMPLATE(8);
}

int arr_idx_set_with_64(void)
{
	ARR_IDX_SET_TEMPLATE(64);
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

	if (!res)
		fprintf(stderr, "%s: FAIL (%s)\n", i->name, i->descr);

	return res;
}

struct test_info tests[] = {
	TEST(sys_word_size,		"checks J64 word storage size"),
	TEST(sys_ptr_size,		"checks J64 pointer storage size"),
	TEST(sys_j64_union_size,	"checks J64 union storage size"),
	TEST(sys_malloc_alignment,	"checks memory allocator pointer alignment"),

	TEST(mk_null,			"tests null literal creation"),
	TEST(mk_false,			"tests false literal creation"),
	TEST(mk_true,			"tests true literal creation"),
	TEST(mk_empty_str,		"tests empty string literal creation"),
	TEST(mk_empty_arr,		"tests empty array literal creation"),
	TEST(mk_empty_obj,		"tests empty object literal creation"),
	TEST(mk_int0,			"tests zero integer creation"),
	TEST(mk_int1,			"tests complemented integer creation"),
	TEST(mk_float,			"tests floating-point number creation"),
	TEST(mk_str_with_0,		"tests string creation with empty string"),
	TEST(mk_str_with_1,		"tests string creation with one character"),
	TEST(mk_str_with_7,		"tests string creation with 7 characters"),
	TEST(mk_str_with_8,		"tests string creation with 8 characters"),
	TEST(mk_str_with_16,		"tests string creation with 16 characters"),
	TEST(mk_arr_with_0,		"tests array creation with zero elements"),
	TEST(mk_arr_with_1,		"tests array creation with one element"),
	TEST(mk_arr_with_8,		"tests array creation with 8 elements"),
	TEST(mk_arr_with_64,		"tests array creation with 64 elements"),

	TEST(int_eq_with_zero,		"tests signed integer equality with zero"),
	TEST(int_eq_with_one_minus,	"tests signed integer equality with minus one"),
	TEST(int_eq_with_one,		"tests signed integer equality with one"),
	TEST(int_eq_with_min,		"tests signed integer equality with minimum value"),
	TEST(int_eq_with_max,		"tests signed integer equality with maximum value"),
	TEST(int_neq_with_min_under,	"tests signed integer unequality with value under minimum"),
	TEST(int_neq_with_max_over,	"tests signed integer unequality with value over maximum"),

	TEST(str_len_with_1,		"tests string length with one character"),
	TEST(str_len_with_7,		"tests string length with 7 characters"),
	TEST(str_len_with_8,		"tests string length with 8 characters"),
	TEST(str_len_with_16,		"tests string length with 16 characters"),
	TEST(str_idx_get_with_1,	"tests indexed string read with one character"),
	TEST(str_idx_get_with_7,	"tests indexed string read with 7 characters"),
	TEST(str_idx_get_with_8,	"tests indexed string read with 8 characters"),
	TEST(str_idx_get_with_16,	"tests indexed string read with 16 characters"),
	TEST(str_idx_set_with_1,	"tests indexed string write with one character"), 
	TEST(str_idx_set_with_7,	"tests indexed string write with 7 characters"), 
	TEST(str_idx_set_with_8,	"tests indexed string write with 8 characters"), 
	TEST(str_idx_set_with_16,	"tests indexed string write with 16 characters"), 
	TEST(str_eq_with_1,		"tests string equality with one character"),
	TEST(str_eq_with_7,		"tests string equality with 7 characters"),
	TEST(str_eq_with_8,		"tests string equality with 8 characters"),
	TEST(str_eq_with_16,		"tests string equality with 16 characters"),

	TEST(arr_len_with_1,		"tests array length with one element"),
	TEST(arr_len_with_8,		"tests array length with 8 elements"),
	TEST(arr_len_with_64,		"tests array length with 64 elements"),
	TEST(arr_idx_get_with_1,	"tests indexed array read with 1 element"),
	TEST(arr_idx_get_with_8,	"tests indexed array read with 8 elements"),
	TEST(arr_idx_get_with_64,	"tests indexed array read with 64 elements"),
	TEST(arr_idx_set_with_1,	"tests indexed array write with one element"),
	TEST(arr_idx_set_with_8,	"tests indexed array write with 8 elements"),
	TEST(arr_idx_set_with_64,	"tests indexed array write with 64 elements")
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

	if (nsuccess == NTESTS) {
		fprintf(stderr, "All %lu tests passed!\n", NTESTS);
		return EXIT_SUCCESS;
	} else {
		fprintf(stderr, "%d out of %lu tests failed.\n", nfail, NTESTS);
		return EXIT_FAILURE;
	}
}
