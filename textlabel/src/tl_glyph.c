#include "tl_glyph.h"

#include <dtex_hash.h>

#include <string.h>

struct glyph_cache {
	struct dtex_hash* hash;

	int size;
	struct tl_glyph_key glyphs[1];
};

static struct glyph_cache* C;

static inline unsigned int 
_hash_func(int hash_sz, void* key) {
	struct tl_glyph_key* hk = (struct tl_glyph_key*)key;
	return (hk->unicode ^ (hk->font * 97) ^ (hk->size * 101) ^ hk->edge) % hash_sz;
}

static inline bool 
_equal_func(void* key0, void* key1) {
	struct tl_glyph_key* hk0 = (struct tl_glyph_key*)key0;
	struct tl_glyph_key* hk1 = (struct tl_glyph_key*)key1;
	return hk0->unicode == hk1->unicode
		&& hk0->font	== hk1->font
		&& hk0->size	== hk1->size
		&& hk0->edge	== hk1->edge;
}

void 
tl_glyph_cache_init(int cap) {
	size_t sz = sizeof(struct gly)

	memset(&C, 0, sizeof(C));
	C.hash = dtex_hash_create(CACHE_SIZE, CACHE_SIZE * 2, 0.5f, _hash_func, _equal_func);
}

struct tl_glyph* 
tl_glyph_gen_char(int unicode, int font, int size, bool edge) {
	
}