#include <stdio.h>
#include <stdint.h>

#include "j64.h"

int is_null(void)
{
	return j64_is_null(j64_null());
}

int is_false(void)
{
	return j64_is_false(j64_false());
}

struct test_info {
	const char *name;
	const char *descr;
	int (*fn)(void);
};

#define TEST(fn, descr) { #fn, descr, fn }

int run_test(struct test_info *i)
{
	int res= i->fn();
	fprintf(stderr, "%s: %s\n", i->name, res ? "ok" : "FAIL");
	return res;
}

struct test_info tests[] = {
	TEST(is_null,	"checks that a created null literal is null"),
	TEST(is_false,	"checks that a created false literal is false")
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

	return 0;
}
