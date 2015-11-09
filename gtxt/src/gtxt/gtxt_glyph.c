#include "gtxt_glyph.h"
#include "gtxt_freetype.h"

#include <dtex_hash.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct glyph_key {
	int unicode;
	int font;
	int size;
	bool edge;
};

struct glyph_bitmap {
	bool valid;

	uint8_t* buf;
	size_t sz;

	struct glyph_bitmap *next;
};

struct glyph {
	struct glyph_key key;

	struct glyph_bitmap* bitmap;
	struct gtxt_glyph_layout layout; 

	struct glyph *prev, *next;
};

struct glyph_cache {
	struct dtex_hash* hash;

	struct glyph_bitmap* bitmap_freelist;
	struct glyph_bitmap *bitmap_head, *bitmap_tail;

	struct glyph* glyph_freelist;
	struct glyph *glyph_head, *glyph_tail;
};

static struct glyph_cache* C;

static inline unsigned int 
_hash_func(int hash_sz, void* key) {
	struct glyph_key* hk = (struct glyph_key*)key;
	return (hk->unicode ^ (hk->font * 97) ^ (hk->size * 101) ^ hk->edge) % hash_sz;
}

static inline bool 
_equal_func(void* key0, void* key1) {
	struct glyph_key* hk0 = (struct glyph_key*)key0;
	struct glyph_key* hk1 = (struct glyph_key*)key1;
	return hk0->unicode == hk1->unicode
		&& hk0->font	== hk1->font
		&& hk0->size	== hk1->size
		&& hk0->edge	== hk1->edge;
}

void 
gtxt_glyph_cache_init(int cap_bitmap, int cap_layout) {
	size_t bitmap_sz = sizeof(struct glyph_bitmap) * cap_bitmap;
	size_t layout_sz = sizeof(struct glyph) * cap_layout;
	size_t sz = sizeof(struct glyph_cache) + bitmap_sz + layout_sz;
	C = (struct glyph_cache*)malloc(sz);
	if (!C) {
		return;
	}
	memset(C, 0, sz);

	C->hash = dtex_hash_create(cap_layout, cap_layout * 2, 0.5f, _hash_func, _equal_func);

	// bitmap
	C->bitmap_freelist = (struct glyph_bitmap*)(C + 1);
	for (int i = 0; i < cap_bitmap - 1; ++i) {
		C->bitmap_freelist[i].next = &C->bitmap_freelist[i + 1];
	}
	C->bitmap_freelist[cap_bitmap - 1].next = NULL;
	
	// layout
	C->glyph_freelist = (struct glyph*)((intptr_t)C->bitmap_freelist + bitmap_sz);
	for (int i = 0; i < cap_layout - 1; ++i) {
		C->glyph_freelist[i].next = &C->glyph_freelist[i+1];
	}
	C->glyph_freelist[cap_layout - 1].next = NULL;
	C->glyph_freelist[0].prev = NULL;
	for (int i = 1; i < cap_layout; ++i) {
		C->glyph_freelist[i].prev = &C->glyph_freelist[i-1];
	}
}

static inline struct glyph*
_new_node() {
	if (!C->glyph_freelist) {
		assert(C->glyph_head);
		struct glyph* g = C->glyph_head;
		C->glyph_head = g->next;

		dtex_hash_remove(C->hash, &g->key);
		if (g->bitmap && g->bitmap->valid) {
			g->bitmap->valid = false;
			g->bitmap->next = C->bitmap_freelist;
			C->bitmap_freelist = g->bitmap;
		}
		g->prev = g->next = NULL;

		C->glyph_freelist = g;
	}

	assert(C->glyph_freelist);
	struct glyph* g = C->glyph_freelist;
	C->glyph_freelist = g->next;

	if (!C->glyph_head) {
		assert(!C->glyph_tail);
		C->glyph_head = C->glyph_tail = g;
		g->prev = g->next = NULL;
	}

	return g;
}

struct gtxt_glyph_layout* 
gtxt_glyph_get_layout(int unicode, int font, int size, bool edge) {
	struct glyph_key key;
	key.unicode = unicode;
	key.font = font;
	key.size = size;
	key.edge = edge;

	struct glyph* g = (struct glyph*)dtex_hash_query(C->hash, &key);
	if (g) {
		return &g->layout;
	} else {
		g = _new_node();
		gtxt_ft_get_layout(unicode, font, size, edge, &g->layout);

		g->key = key;
		dtex_hash_insert(C->hash, &g->key, g, true);

		return &g->layout;
	}
}

uint8_t* 
gtxt_glyph_get_bitmap(int unicode, int font, int size, bool edge, struct gtxt_glyph_layout* layout) {
	struct glyph_key key;
	key.unicode = unicode;
	key.font = font;
	key.size = size;
	key.edge = edge;

	struct glyph* g = (struct glyph*)dtex_hash_query(C->hash, &key);
	if (g) {
		if (g->prev) {
			g->prev->next = g->next;
		}
		g->next = NULL;
		assert(C->glyph_tail);
		C->glyph_tail->next = g;
		*layout = g->layout;
	} else {
		g = _new_node();
		g->bitmap->valid = false;

		g->key = key;
		dtex_hash_insert(C->hash, &g->key, g, true);
	}

	if (!g->bitmap) {
		if (!C->bitmap_freelist) {
			assert(C->bitmap_head);
			struct glyph_bitmap* bmp = C->bitmap_head;
			C->bitmap_head = bmp->next;

			bmp->valid = false;
			bmp->next = C->bitmap_freelist;
			C->bitmap_freelist = bmp;
		}
		g->bitmap = C->bitmap_freelist;
		C->bitmap_freelist = C->bitmap_freelist->next;
		g->bitmap->valid = false;
	}
	if (!g->bitmap->valid) {
		uint8_t* buf = gtxt_ft_gen_char(unicode, font, size, edge, &g->layout);
		*layout = g->layout;
		size_t sz = g->layout.sizer.width * g->layout.sizer.height;
		if (sz > g->bitmap->sz) {
			free(g->bitmap->buf);
			g->bitmap->buf = malloc(sz);
			g->bitmap->sz = sz;
		}

		memcpy(g->bitmap->buf, buf, sz);
		g->bitmap->valid = true;

		if (!C->bitmap_tail) {
			assert(!C->bitmap_head);
			C->bitmap_head = C->bitmap_tail = g->bitmap;
		} else {
			C->bitmap_tail->next = g->bitmap;
			C->bitmap_tail = g->bitmap;
		}
		g->bitmap->next = NULL;
	}

	return g->bitmap->buf;
}