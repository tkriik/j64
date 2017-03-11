#include <stdio.h>

#include "j64.h"

void print_header(const char *s)
{
	fprintf(stderr, "---------------- %s ----------------\n\n", s);
}

int main(void)
{
	print_header("LITERALS");
	j64_dbg(j64_null());
	j64_dbg(j64_false());
	j64_dbg(j64_true());
	j64_dbg(j64_empty_str());
	j64_dbg(j64_empty_arr());
	j64_dbg(j64_empty_obj());

	print_header("INTEGERS");
	j64_dbg(j64_uint(0x100));
	j64_dbg(j64_int(0x100));

	print_header("STRINGS");
	j64_dbg(j64_str(""));
	j64_dbg(j64_str("1"));
	j64_dbg(j64_str("1234567"));
	j64_dbg(j64_str("12345678"));

	return 0;
}
