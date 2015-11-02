#include "gtxt_label.h"
#include "gtxt_layout.h"
#include "gtxt_glyph.h"
#include "gtxt_richtext.h"
#include "gtxt_render.h"

#include <dtex_array.h>

#include <string.h>
#include <assert.h>

static struct dtex_array* UNICODE_BUF;

static inline int
_unicode_len(const char chr) {
	uint8_t c = (uint8_t)chr;
	if ((c&0x80) == 0) {
		return 1;
	} else if ((c&0xe0) == 0xc0) {
		return 2;
	} else if ((c&0xf0) == 0xe0) {
		return 3;
	} else if ((c&0xf8) == 0xf0) {
		return 4;
	} else if ((c&0xfc) == 0xf8) {
		return 5;
	} else {
		return 6;
	}
}

//static inline int
//_get_unicode_and_char(const char* str, int n, char* utf8) {
//	int i;
//	utf8[0] = str[0];
//	int unicode = utf8[0] & ((1 << (8 - n)) - 1);
//	for (i = 1; i < n; ++i) {
//		utf8[i] = str[i];
//		unicode = unicode << 6 | ((uint8_t)utf8[i] & 0x3f);
//	}
//	utf8[i] = 0;
//	return unicode;
//}

static inline int
_get_unicode(const char* str, int n) {
	int unicode = str[0] & ((1 << (8 - n)) - 1);
	for (int i = 1; i < n; ++i) {
		unicode = unicode << 6 | ((uint8_t)str[i] & 0x3f);
	}
	return unicode;
}

static inline void
_draw_glyph_cb(int unicode, float x, float y, void* ud) {
	struct gtxt_label_style* style = (struct gtxt_label_style*)ud;	

	struct gtxt_render_style d_style;
	d_style.color = style->color;
	d_style.size = style->font_size;
	d_style.font = style->font;
	d_style.edge = style->edge;

	gtxt_draw_glyph(unicode, &d_style, x, y);
}

struct layout_pos {
	int unicode;
	float x, y;
};

struct layout_result_with_idx {
	struct layout_pos* result;
	int idx;
};

static inline int
_draw_richtext_glyph_cb(const char* str, struct gtxt_richtext_style* style, void* ud) {
	struct layout_result_with_idx* params = (struct layout_result_with_idx*)ud;

	struct gtxt_render_style d_style;
	d_style.color = style->color;
	d_style.size = style->size;
	d_style.font = style->font;
	d_style.edge = style->edge;

	int len = _unicode_len(str[0]);
	int unicode = _get_unicode(str, len);

	struct layout_pos* pos = &params->result[params->idx++];
	assert(pos->unicode == unicode);
	gtxt_draw_glyph(unicode, &d_style, pos->x, pos->y);

	return len;
}

void 
gtxt_label_draw(const char* str, struct gtxt_label_style* style) {
	if (!UNICODE_BUF) {
		UNICODE_BUF = dtex_array_create(128, sizeof(int));
	}

	int str_len = strlen(str);
	for (int i = 0; i < str_len; ) {
		int len = _unicode_len(str[i]);
		int unicode = _get_unicode(str + i, len);
		dtex_array_add(UNICODE_BUF, &unicode);
		i += len;
	}

	gtxt_layout_begin(style);
	gtxt_layout_multi(UNICODE_BUF);					// layout
	gtxt_layout_traverse(_draw_glyph_cb, style);	// draw
	gtxt_layout_end();
}

static inline int
_layout_richtext_glyph_cb(const char* str, struct gtxt_richtext_style* style, void* ud) {
	int* count = (int*)ud;
	++*count;

	int len = _unicode_len(str[0]);
	int unicode = _get_unicode(str, len);
	gtxt_layout_single(unicode, style);
	return len;
}

static inline void
_get_layout_result_cb(int unicode, float x, float y, void* ud) {
	struct layout_result_with_idx* params = (struct layout_result_with_idx*)ud;
	params->result[params->idx].unicode = unicode;
	params->result[params->idx].x = x;
	params->result[params->idx].y = y;
	++params->idx;
}

void 
gtxt_label_draw_richtext(const char* str, struct gtxt_label_style* style) {
	if (!UNICODE_BUF) {
		UNICODE_BUF = dtex_array_create(128, sizeof(int));
	}

	gtxt_layout_begin(style);

	// layout
	int count = 0;
	gtxt_richtext_parser(str, style, _layout_richtext_glyph_cb, &count);	// layout

	// get layout
	struct layout_pos pos[count];
	struct layout_result_with_idx params;
	params.result = pos;
	params.idx = 0;
	gtxt_layout_traverse(_get_layout_result_cb, &params);

	gtxt_layout_end();

	// draw
	params.idx = 0;
	gtxt_richtext_parser(str, style, _draw_richtext_glyph_cb, &params);	// layout
}