#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "j64.h"

#include <stdio.h>
#include <inttypes.h>

j64_t j64_float(double f)
{
	j64_t j;
	j.f = f;
	j.w |= J64_TAG_PRIM_FLOAT;
	return j;
}

j64_t j64_istrn(const char *s, size_t len)
{
	/* Truncate length if necessary. */
	len = len <= J64_ISTR_LEN_MAX ? len : J64_ISTR_LEN_MAX;

	j64_t j = _J64_INITIALIZER;
	_j64_prim_tag_set(j, J64_TAG_PRIM_ISTR);
	_j64_istr_len_set(j, len);
	memcpy(_j64_istr_buf(j), s, len);

	/* POST */
	assert(j64_is_istr(j));
	assert(len <= J64_ISTR_LEN_MAX);
	assert(j64_istr_len(j) == len);
	assert(strncmp((const char *)_j64_istr_buf(j), s, len) == 0);

	return j;
}

j64_t j64_istr(const char *s)
{
	size_t len = strlen(s);
	return j64_istrn(s, len);
}

j64_t j64_bstrn(const char *s, const size_t len)
{
	j64_t j = _J64_INITIALIZER;
	// TODO: check for overflow
	j.p = malloc(_J64_BSTR_HDR_SIZEOF + len);
	if (j.p == NULL)
		return j64_null();
	_j64_bstr_len_set(j, len);
	memcpy(_j64_bstr_buf(j), s, len);
	_j64_prim_tag_set(j, J64_TAG_PRIM_BSTR);

	/* POST */
	assert(j64_is_bstr(j));
	assert(j64_bstr_len(j) == len);
	assert(strncmp((const char *)_j64_bstr_buf(j), s, len) == 0);

	return j;
}

j64_t j64_bstr(const char *s)
{
	size_t len = strlen(s);
	return j64_bstrn(s, len);
}

j64_t j64_strn(const char *s, const size_t len)
{
	if (len == 0)
		return j64_estr();

	/* PRE */
	assert(s != NULL);

	j64_t j = _J64_INITIALIZER;

	/*
	 * Construct immediate string if string fits into 56 bits,
	 * otherwise construct a boxed string.
	 */
	if (len <= J64_ISTR_LEN_MAX)
		j = j64_istrn(s, len);
	else
		j = j64_bstrn(s, len);

	/* POST */
	assert(j64_is_str(j));

	return j;
}

j64_t j64_str(const char *s)
{
	size_t len = strlen(s);
	return j64_strn(s, len);
}

#define MIN(a, b) ((a) < (b) ? (a) : (b))

size_t j64_istr_get(j64_t j, char *dst, const size_t dst_len)
{
	/* PRE */
	assert(j64_is_istr(j));
	assert(dst != NULL);

	if (dst_len == 0)
		return 0;

	size_t ncopy = MIN(j64_istr_len(j), dst_len - 1);
	memcpy(dst, _j64_istr_buf(j), ncopy);
	dst[ncopy] = '\0';

	/* POST */
	assert(strncmp((const char *)_j64_istr_buf(j), dst, ncopy) == 0);

	return ncopy;
}

size_t j64_bstr_get(j64_t j, char *dst, const size_t dst_len)
{
	/* PRE */
	assert(j64_is_bstr(j));
	assert(dst != NULL);

	if (dst_len == 0)
		return 0;

	size_t ncopy = MIN(j64_bstr_len(j), dst_len - 1);
	memcpy(dst, _j64_bstr_buf(j), ncopy);
	dst[ncopy] = '\0';

	/* POST */
	assert(strncmp((const char *)_j64_bstr_buf(j), dst, ncopy) == 0);

	return ncopy;
}

#define J64_ARR_CNT_MIN 6

j64_t j64_arr(const j64_t *js, const size_t len)
{
	if (len == 0)
		return j64_earr();

	/* PRE */
	assert(js != NULL);

	j64_t j;
	// TODO: more sane initial capacity
	size_t cap = len + (len / 2);
	// TODO: check for overflow
	j.p = malloc(_J64_ARR_HDR_SIZEOF + cap * sizeof(j));
	if (j.p == NULL)
		return j64_null();

	struct _j64_arr_hdr *hdr = j.p;
	hdr->len = len;
	hdr->cap = cap;
	j64_t *elems = _j64_arr_buf(j);
	for (size_t i = 0; i < len; i++)
		elems[i] = js[i];

	_j64_prim_tag_set(j, J64_TAG_PRIM_ARR);

	/* POST */
	assert(j.p != NULL);
	assert(j64_is_barr(j));
	assert(j64_arr_len(j) == len);
	assert(j64_arr_cap(j) == cap);

	return j;
}

int j64_eq(j64_t j0, j64_t j1)
{
	// TODO: boxed comparison
	return j0.w == j1.w;
}

void j64_free(j64_t j)
{
	switch (j64_prim_tag(j)) {
	case J64_TAG_PRIM_BSTR:
		free(_j64_bstr_hdr(j));
		break;
	case J64_TAG_PRIM_ARR:
		free(_j64_arr_hdr(j));
		break;
	case J64_TAG_PRIM_OBJ:
		// TODO: _j64_obj_hdr
		break;
	}
}

void j64_dbg(j64_t j)
{
	struct _j64_bstr_hdr *hdr;

	switch (j64_prim_tag(j)) {
	case J64_TAG_PRIM_LIT:
		fprintf(stderr, "literal: ");
		switch (j64_subtag_lit(j)) {
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
			    j64_subtag_lit(j));
			break;
		}
		break;
	case J64_TAG_PRIM_INT0:
	case J64_TAG_PRIM_INT1:
		fprintf(stderr, "int: %" PRId64 " ", j64_int_get(j));
		break;
	case J64_TAG_PRIM_FLOAT:
		fprintf(stderr, "float: %lf ", j.f);
		break;
	case J64_TAG_PRIM_ISTR:
		fprintf(stderr, "istr (%llu): ", j64_istr_len(j));
		for (size_t i = 1; i <= j64_istr_len(j); i++)
			fputc(j.b[8 - i], stderr);
		fputc(' ', stderr);
		break;
	case J64_TAG_PRIM_BSTR:
		hdr = _j64_get_ptr(j);
		fprintf(stderr, "bstr (%zu): %p, %s ", hdr->len, (void *)hdr, &hdr->buf);
		break;
	case J64_TAG_PRIM_ARR:
		fprintf(stderr, "arr: %p", _j64_get_ptr(j));
		break;
	case J64_TAG_PRIM_OBJ:
		fprintf(stderr, "obj: %p", _j64_get_ptr(j));
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
