#include "tl_glyph.h"
#include "tl_freetype.h"

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

struct glyph {
	struct glyph_key key;

	uint8_t* buf;
	struct tl_glyph_layout layout; 

	struct glyph *prev, *next;
};

struct glyph_cache {
	struct dtex_hash* hash;

	struct glyph* freenode;

	struct glyph *head, *tail;
	struct glyph allnodes[1];
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
tl_glyph_cache_init(int cap) {
	size_t sz = sizeof(struct glyph_cache) + sizeof(struct glyph_key) * cap;
	C = (struct glyph_cache*)malloc(sz);
	if (!C) {
		return;
	}

	memset(&C, 0, sz);

	C->hash = dtex_hash_create(cap, cap * 2, 0.5f, _hash_func, _equal_func);

	C->freenode = &C->allnodes[0];

	for (int i = 0; i < cap - 1; ++i) {
		C->allnodes[i].next = &C->allnodes[i+1];
	}
	C->allnodes[cap - 1].next = NULL;

	C->allnodes[0].prev = NULL;
	for (int i = 1; i < cap; ++i) {
		C->allnodes[i].prev = &C->allnodes[i-1];
	}
}

uint8_t* 
tl_glyph_gen_char(int unicode, int font, int size, bool edge, struct tl_glyph_layout* layout) {
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
		assert(C->tail);
		C->tail->next = g;
		*layout = g->layout;
		return g->buf;
	}

	if (!C->freenode) {
		assert(C->head);
		struct glyph* g = C->head;
		C->head = g->next;

		dtex_hash_remove(C->hash, &g->key);
		free(g->buf); 
		g->buf = NULL;
		g->prev = g->next = NULL;

		C->freenode = g;
	}

	assert(C->freenode);
	g = C->freenode;
	C->freenode = g->next;

	if (!C->head) {
		assert(!C->tail);
		C->head = C->tail = g;
		g->prev = g->next = NULL;
	}

	g->key = key;
	g->buf = tl_ft_gen_char(unicode, font, size, edge, &g->layout);

	return g->buf;
}