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

j64_t j64_str(const char *s)
{
	size_t len = strlen(s);
	return j64_strn(s, len);
}

j64_t j64_strn(const char *s, size_t len)
{
	/* PRE */
	assert(s != NULL);

	/* Return immediate empty string if necessary. */
	if (*s == '\0')
		return j64_estr();

	j64_t j = {0};

	/*
	 * Construct immediate string if string fits into 56 bits,
	 * otherwise allocate and construct a boxed string.
	 */
	if (len <= J64_ISTR_LEN_MAX) {
		memcpy(_j64_istr_buf(j), s, len);
		_j64_istr_len_set(j, len);
		_j64_prim_tag_set(j, J64_TAG_PRIM_ISTR);
	} else {
		j.p = malloc(_J64_BSTR_HDR_SIZEOF + len);
		if (j.p == NULL)
			return j64_null();

		struct _j64_bstr_hdr *h = j.p;
		h->len = len;
		memcpy(_j64_bstr_buf(j), s, len);
		_j64_prim_tag_set(j, J64_TAG_PRIM_BSTR);
	}

	/* POST */
	assert(j64_is_str(j));
	assert(j64_str_len(j) == len);
	assert(strncmp((const char *)_j64_str_buf(j), s, len) == 0);

	return j;
}

#define MIN(a, b) ((a) < (b) ? (a) : (b))

size_t j64_str_get(const j64_t j, char *dst, const size_t dst_size)
{
	/* PRE */
	assert(j64_is_str(j));
	assert(dst != NULL);

	if (dst_size == 0)
		return 0;

	size_t dst_len = dst_size - 1;

	const _j64_byte_t *src;
	size_t src_len;
	switch (j64_prim_tag(j)) {
	case J64_TAG_PRIM_LIT:
		switch (j64_subtag_lit(j)) {
		case J64_SUBTAG_LIT_ESTR:
			*dst = '\0';
			return 0;
		default:
			return 0;
		}
	case J64_TAG_PRIM_ISTR:
		src = _j64_istr_buf(j);
		src_len = j64_istr_len(j);
		break;
	case J64_TAG_PRIM_BSTR:
		src = _j64_bstr_buf(j);
		src_len = j64_bstr_len(j);
		break;
	default:
		return 0;
	}

	const size_t min_len = MIN(src_len, dst_len);
	memcpy(dst, src, min_len);
	dst[min_len] = '\0';

	/* POST */
	assert(strncmp((const char*)_j64_str_buf(j), dst, min_len) == 0);

	return min_len;
}

#define J64_ARR_CNT_MIN 6

j64_t j64_arr(const j64_t *js, const size_t cnt)
{
	if (cnt == 0)
		return j64_earr();

	/* PRE */
	assert(js != NULL);

	j64_t j;
	// TODO: more sane initial capacity
	size_t cap = cnt + (cnt / 2);
	// TODO: check for overflow
	j.p = malloc(_J64_ARR_HDR_SIZEOF + cap * sizeof(j));
	if (j.p == NULL)
		return j64_null();

	struct _j64_arr_hdr *hdr = j.p;
	hdr->cnt = cnt;
	hdr->cap = cap;
	j64_t *elems = _j64_arr_buf(j);
	for (size_t i = 0; i < cnt; i++)
		elems[i] = js[i];

	_j64_prim_tag_set(j, J64_TAG_PRIM_ARR);

	/* POST */
	assert(j.p != NULL);
	assert(j64_is_barr(j));
	assert(j64_arr_cnt(j) == cnt);
	assert(j64_arr_cap(j) == cap);

	return j;
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
		fprintf(stderr, "bstr (%llu): %p, %s ", hdr->len, (void *)hdr, &hdr->buf);
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
