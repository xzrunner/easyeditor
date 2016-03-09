#include "sp_null.h"
#include "sp_region.h"

#include <stdlib.h>
#include <string.h>

#define MAX_NODE 1024

struct sp_null {
	void* ud[MAX_NODE];
	struct sp_region region[MAX_NODE];
	
	int count;
};

struct sp_null* 
sp_null_create() {
	int sz = sizeof(struct sp_null);
	struct sp_null* null = (struct sp_null*)malloc(sz);
	memset(null, 0, sz);
	return null;
}

void 
sp_null_release(struct sp_null* null) {
	free(null);
}

void 
sp_null_insert(struct sp_null* null, struct sp_region* r, void* ud) {
	if (null->count >= MAX_NODE) {
		return;
	}

	null->ud[null->count] = ud;
	null->region[null->count] = *r;
	++null->count;
}

bool 
sp_null_remove(struct sp_null* null, void* ud) {
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
sp_null_clear(struct sp_null* null) {
	null->count = 0;
}

void 
sp_null_query(struct sp_null* null, struct sp_region* r, void** ud, int* count) {
	*count = 0;
	for (int i = 0; i < null->count; ++i) {
		if (REGION_INTERSECT(null->region[i], *r)) {
			ud[(*count)++] = null->ud[i];
		}
	}
}

void 
sp_null_debug_draw(struct sp_null* null, void (*render)(float xmin, float ymin, float xmax, float ymax)) {
	for (int i = 0; i < null->count; ++i) {
		struct sp_region* r = &null->region[i];
		render(r->xmin, r->ymin, r->xmax, r->ymax);
	}
}