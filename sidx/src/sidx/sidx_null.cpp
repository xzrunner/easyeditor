#include "sidx_null.h"
#include "sidx_region.h"

#include <stdlib.h>
#include <string.h>

#define MAX_NODE 1024

struct sidx_null {
	void* ud[MAX_NODE];
	struct sidx_region region[MAX_NODE];
	
	int count;
};

struct sidx_null* 
sidx_null_create() {
	int sz = sizeof(struct sidx_null);
	struct sidx_null* null = (struct sidx_null*)malloc(sz);
	memset(null, 0, sz);
	return null;
}

void 
sidx_null_release(struct sidx_null* null) {
	free(null);
}

void 
sidx_null_insert(struct sidx_null* null, struct sidx_region* r, void* ud) {
	if (null->count >= MAX_NODE) {
		return;
	}

	null->ud[null->count] = ud;
	null->region[null->count] = *r;
	++null->count;
}

bool 
sidx_null_remove(struct sidx_null* null, void* ud) {
	for (int i = 0; i < null->count; ++i) {
		if (null->ud[i] == ud) {
			null->ud[i] = null->ud[null->count-1];
			null->region[i] = null->region[null->count-1];
			--null->count;
			return true;
		}
	}
	return false;
}

void 
sidx_null_clear(struct sidx_null* null) {
	null->count = 0;
}

void 
sidx_null_query(struct sidx_null* null, struct sidx_region* r, void** ud, int* count) {
	*count = 0;
	for (int i = 0; i < null->count; ++i) {
		if (REGION_INTERSECT(null->region[i], *r)) {
			ud[(*count)++] = null->ud[i];
		}
	}
}

void 
sidx_null_debug_draw(struct sidx_null* null, void (*render)(float xmin, float ymin, float xmax, float ymax)) {
	for (int i = 0; i < null->count; ++i) {
		struct sidx_region* r = &null->region[i];
		render(r->xmin, r->ymin, r->xmax, r->ymax);
	}
}