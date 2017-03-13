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

	j64_t j;
	size_t len = strlen(s);

	/*
	 * Construct immediate string if string fits into 56 bits,
	 * otherwise allocate and construct a boxed string.
	 */
	if (len < 8) {
		memcpy(&j.b[1], s, len);
		J64_SET_SUBTAG_STR_LEN(j, len);
		J64_SET_PRIM_TAG(j, J64_TAG_PRIM_ISTR);
	} else {
		j.p = malloc(J64_BSTR_HDR_SIZEOF + len);
		if (j.p == NULL)
			return j64_null();

		struct j64_bstr_hdr *h = j.p;
		h->len = len;
		memcpy(J64_BSTR_HDR_BUF(h), s, len);
		J64_SET_PRIM_TAG(j, J64_TAG_PRIM_BSTR);
	}

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
		hdr = J64_GET_PTR(j);
		fprintf(stderr, "bstr: %p, %s ", (void *)hdr, J64_BSTR_HDR_BUF(hdr));
		break;
	case J64_TAG_PRIM_ARR:
		fprintf(stderr, "arr: %p", J64_GET_PTR(j));
		break;
	case J64_TAG_PRIM_OBJ:
		fprintf(stderr, "obj: %p", J64_GET_PTR(j));
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
