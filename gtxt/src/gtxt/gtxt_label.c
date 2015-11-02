#include "gtxt_label.h"
#include "gtxt_layout.h"
#include "gtxt_glyph.h"

#include <dtex_array.h>

#include <string.h>

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
_draw_glyph(int unicode, int font, int size, bool edge, int x, int y) {
	struct gtxt_glyph_layout layout;
	uint8_t* buf = gtxt_glyph_get_bitmap(unicode, font, size, edge, &layout);
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

	gtxt_layout(UNICODE_BUF, style);

	gtxt_layout_traverse(_draw_glyph);

	gtxt_layout_end();
}

static inline int
_draw_richtext_glyph(const char* str, struct gtxt_richtext_style* style) {
	int len = _unicode_len(str[i]);
	int unicode = _get_unicode(str + i, len);

	

	return len;
}

void 
gtxt_label_draw_richtext(const char* str, struct gtxt_label_style* style) {
	
}