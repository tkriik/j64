#include <stdlib.h>
#include <string.h>

#include "j64.h"

#include <stdio.h>
#include <inttypes.h>

j64_t j64_float(j64_float_t f)
{
	j64_t j;
	j.f = f;
	j.u |= J64_TAG_PRIM_FLOAT;
	return j;
}

j64_t j64_str(const char *s)
{
	/* Return immediate empty string if necessary. */
	if (*s == '\0')
		return j64_empty_str();

	/* Compute the length of the source string up to 8 bytes. */
	size_t len = 0;
	for (size_t i = 0; i < 8; i++) {
		if (s[i] == '\0')
			break;
		else
			len++;
	}

	j64_t j;

	/*
	 * Jump to boxed string construction if initial length is 8.
	 * Otherwise fall through a switch with the string bytes
	 * copied to the word in reverse order.
	 */
	switch (len) {
	case 8: goto j64_str_mk_boxed;
	case 7: j64_istr_set_at(j, 6, s[6]);
	case 6: j64_istr_set_at(j, 5, s[5]);
	case 5: j64_istr_set_at(j, 4, s[4]);
	case 4: j64_istr_set_at(j, 3, s[3]);
	case 3: j64_istr_set_at(j, 2, s[2]);
	case 2: j64_istr_set_at(j, 1, s[1]);
	case 1: j64_istr_set_at(j, 0, s[0]);
	}

	/* Set string length and type. */
	J64_SET_SUBTAG_STR_LEN(j, len);
	J64_SET_PRIM_TAG(j, J64_TAG_PRIM_ISTR);

	return j;

j64_str_mk_boxed:

	/* Compute the rest of the length. */
	len = 8 + strlen(s + 8);

	j.p = malloc(J64_BSTR_HDR_SIZEOF + len + 1);
	if (j.p == NULL)
		return j64_null();

	struct j64_bstr_hdr *h = j.p;
	h->len = len;
	memcpy(J64_BSTR_HDR_BUF(h), s, len + 1);

	J64_SET_PRIM_TAG(j, J64_TAG_PRIM_BSTR);

	return j;
}


void j64_dbg(j64_t j)
{
	struct j64_bstr_hdr *hdr;

	switch (J64_GET_PRIM_TAG(j)) {
	case J64_TAG_PRIM_LIT:
		fprintf(stderr, "literal: ");
		switch (J64_GET_SUBTAG_LIT(j)) {
		case J64_SUBTAG_LIT_NULL:
			fprintf(stderr, "null ");
			break;
		case J64_SUBTAG_LIT_FALSE:
			fprintf(stderr, "false ");
			break;
		case J64_SUBTAG_LIT_TRUE:
			fprintf(stderr, "true ");
			break;
		case J64_SUBTAG_LIT_ESTR:
			fprintf(stderr, "\"\" (empty string) ");
			break;
		case J64_SUBTAG_LIT_EARR:
			fprintf(stderr, "[] (empty array) ");
			break;
		case J64_SUBTAG_LIT_EOBJ:
			fprintf(stderr, "{} (empty object)");
			break;
		default:
			fprintf(stderr, "<UNKNOWN LITERAL SUBTAG (%" PRIx64 ")> ",
			    J64_GET_SUBTAG_LIT(j));
			break;
		}
		break;
	case J64_TAG_PRIM_INT0:
	case J64_TAG_PRIM_INT1:
		fprintf(stderr, "int: %" PRId64 " ", j64_get_int(j));
		break;
	case J64_TAG_PRIM_FLOAT:
		fprintf(stderr, "float: %lf ", j.f);
		break;
	case J64_TAG_PRIM_ISTR:
		fprintf(stderr, "istr (%llu): ", J64_GET_SUBTAG_STR_LEN(j));
		for (size_t i = 1; i <= J64_GET_SUBTAG_STR_LEN(j); i++)
			fputc(j.b[8 - i], stderr);
		fputc(' ', stderr);
		break;
	case J64_TAG_PRIM_BSTR:
		hdr = j64_get_ptr(j);
		fprintf(stderr, "bstr: %p, %s ", (void *)hdr, J64_BSTR_HDR_BUF(hdr));
		break;
	case J64_TAG_PRIM_ARR:
		fprintf(stderr, "arr: %p", j64_get_ptr(j));
		break;
	case J64_TAG_PRIM_OBJ:
		fprintf(stderr, "obj: %p", j64_get_ptr(j));
		break;
	default:
		fprintf(stderr, "<UNKNOWN PRIMARY TAG> ");
	}

	fprintf(stderr, "\n\t");

	for (size_t byte = 8; byte > 0; byte--) {
		for (size_t bit = 8; bit > 0; bit--) {
			fprintf(stderr, "%d", (j.b[byte - 1] >> (bit - 1)) & 1);
		}
		fprintf(stderr, "\n\t");
	}

	fputc('\n', stderr);
}
