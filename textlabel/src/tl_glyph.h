#ifdef __cplusplus
extern "C"
{
#endif

#ifndef textlabel_glyph_h
#define textlabel_glyph_h

#include "tl_typedef.h"

#include <stdbool.h>

struct tl_glyph_key {
	int unicode;
	int font;
	int size;
	bool edge;
};

struct tl_glyph {
	struct tl_glyph_key key;

	uint8_t* buf;
	struct tl_glyph_layout layout; 
};

void tl_glyph_cache_init(int cap);

struct tl_glyph* tl_glyph_gen_char(int unicode, int font, int size, bool edge);

#endif // textlabel_glyph_h

#ifdef __cplusplus
}
#endif