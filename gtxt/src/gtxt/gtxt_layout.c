#include "gtxt_layout.h"
#include "gtxt_glyph.h"
#include "gtxt_label.h"
#include "gtxt_richtext.h"

#include <dtex_array.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct glyph {
	int unicode;

	float x, y;
	float w, h;

	float out_width;

	struct glyph* next;
};

struct row {
	float width;

	struct glyph *head, *tail;

	struct row* next;
};

struct layout {
	struct gtxt_label_style* style;

	struct row* head;

	struct glyph* glyph_freelist;
	size_t glyph_cap;

	struct row* row_freelist;
	size_t row_cap;

	float row_height;
	float tot_height;
	float curr_width;

	struct row* curr_row;
};

static struct layout L;

static inline bool
_prepare_glyph_freelist(int cap) {
	if (cap <= L.glyph_cap || cap <= 0) {
		return true;
	}

	L.glyph_cap = cap;
	if (L.glyph_freelist) {
		free(L.head->head);
	}

	size_t sz = sizeof(struct glyph) * cap;
	L.glyph_freelist = (struct glyph*)malloc(sz);
	if (!L.glyph_freelist) {
		return false;
	}
	memset(L.glyph_freelist, 0, sz);

	for (int i = 0; i < cap - 1; ++i) {
		L.glyph_freelist[i].next = &L.glyph_freelist[i + 1];
	}
	L.glyph_freelist[cap - 1].next = NULL;

	return true;
}

static inline bool
_prepare_row_freelist(int cap) {
	if (cap <= L.row_cap || cap <= 0) {
		return true;
	}

	L.row_cap = cap;
	if (L.row_freelist) {
		free(L.head);
	}

	size_t sz = sizeof(struct row) * cap;
	L.row_freelist = (struct row*)malloc(sz);
	if (!L.row_freelist) {
		return false;
	}
	memset(L.row_freelist, 0, sz);

	for (int i = 0; i < cap - 1; ++i) {
		L.row_freelist[i].next = &L.row_freelist[i + 1];
	}
	L.row_freelist[cap - 1].next = NULL;

	return true;
}

static inline struct row*
_new_row() {
	struct row* r = L.row_freelist;
	if (!r) {
		_prepare_row_freelist(L.row_cap * 2);
		return NULL;
	} else {
		struct row* r = L.row_freelist;
		assert(r);
		L.row_freelist = r->next;
		r->next = NULL;
		return r;
	}
}

void 
gtxt_layout_begin(struct gtxt_label_style* style) {
	L.style = style;

	L.row_height = 0;
	L.tot_height = 0;
	L.curr_width = 0;

	_prepare_row_freelist(16);

	L.curr_row = _new_row();
	L.head = L.curr_row;
}

void 
gtxt_layout_end() {
	struct row* last_row = L.head;
	while (last_row->next) {
		last_row = last_row->next;
	}
	last_row->next = L.row_freelist;
	L.row_freelist = L.head;

	L.glyph_freelist = L.head->head;

	struct glyph* last_tail = NULL;
	struct row* r = L.head;
	while (r) {
		if (last_tail) {
			last_tail->next = r->head;
		}
		last_tail = r->tail;

		r->head = NULL;
		r->tail = NULL;

		r = r->next;
	}
}

static inline struct glyph*
_new_glyph() {
	struct glyph* g = L.glyph_freelist;

	if (!g) {

		int zz = 0;
	}

	assert(g);
	L.glyph_freelist = g->next;
	g->next = NULL;
	return g;
}

void 
gtxt_layout_single(int unicode, struct gtxt_richtext_style* style) {
	int font, size;
	bool edge;
	if (style) {
		font = style->font;
		size = style->size;
		edge = style->edge;
	} else {
		font = L.style->font;
		size = L.style->font_size;
		edge = L.style->edge;
	}
	struct gtxt_glyph_layout* g_layout = gtxt_glyph_get_layout(unicode, font, size, edge);

	if (L.row_height == 0) {
		L.row_height = g_layout->metrics_height;
	} else {
		assert(L.row_height == g_layout->metrics_height);
	}

	float w = g_layout->advance * (1 + L.style->space_h);
	if (unicode == '\n' || L.curr_width + w > L.style->width) {
		float h = g_layout->metrics_height * (1 + L.style->space_v);
		L.curr_row->width = L.curr_width;
		L.tot_height += h;
		L.curr_width = 0;

		if (L.tot_height > L.style->height) {
			return;
		}

		struct row* prev = L.curr_row;
		L.curr_row = _new_row();
		if (!L.curr_row) {
			return;
		}
		prev->next = L.curr_row;
		L.curr_row->next = NULL;
	} else {
		struct glyph* g = _new_glyph();
		assert(g);
		g->unicode = unicode;

		g->x = g_layout->bearing_x;
		g->y = g_layout->bearing_y;
		g->w = g_layout->sizer.width;
		g->h = g_layout->sizer.height;

		g->out_width = w;

		L.curr_width += w;
		if (!L.curr_row->head) {
			assert(!L.curr_row->tail);
			L.curr_row->head = L.curr_row->tail = g;
		} else {
			L.curr_row->tail->next = g;
			L.curr_row->tail = g;
		}
	}
}

void 
gtxt_layout_multi(struct dtex_array* unicodes) {
	int glyph_sz = dtex_array_size(unicodes);
	bool succ = _prepare_glyph_freelist(glyph_sz);
	if (!succ) {
		return;
	}

	for (int i = 0; i < glyph_sz; ++i) {
		int unicode = *(int*)dtex_array_fetch(unicodes, i);
		gtxt_layout_single(unicode, NULL);
	}
}

void 
gtxt_layout_traverse(void (*cb)(int unicode, float x, float y, float w, float h, void* ud), void* ud) {
	float x, y;

	switch (L.style->align_v) {
	case VA_TOP: case VA_AUTO:
		y = L.style->height * 0.5f - L.row_height;
		break;
	case VA_BOTTOM:
		y = -L.style->height * 0.5f + L.tot_height - L.row_height;
		break;
	case VA_CENTER:
		y = L.tot_height * 0.5f - L.row_height;
		break;
	default:
		assert(0);
	}

	struct row* r = L.head;
	while (r) {
		switch (L.style->align_h) {
		case HA_LEFT: case HA_AUTO:
			x = -L.style->width * 0.5f;
			break;
		case HA_RIGHT:
			x = L.style->width * 0.5f - r->width;
			break;
		case HA_CENTER:
			x = -r->width * 0.5f;
			break;
		}

		struct glyph* g = r->head;
		while (g) {
			cb(g->unicode, x + g->x + g->w * 0.5f, y + g->y - g->h * 0.5f, g->w, g->h, ud);
			x += g->out_width;
			g = g->next;
		}

		y -= L.row_height;
		r = r->next;
	}
}