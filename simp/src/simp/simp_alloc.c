#include "simp_alloc.h"

#include <stdlib.h>

struct simp_allocator {
	char* buffer;
	int cap;

	struct simp_allocator* next;
};

#define BUF_SIZE (1 << 16)

static struct simp_allocator* freelist = NULL;

void* 
simp_alloc(struct simp_allocator* allocator, int sz) {
	if (sz <= 0) {
		return NULL;
	}
	if (allocator->cap < sz) {
		return NULL;
	}
	void* ret = allocator->buffer;
	allocator->buffer += sz;
	allocator->cap -= sz;
	return ret;
}

struct simp_allocator* 
simp_create_allocator() {
	struct simp_allocator* ret = NULL;
	if (freelist) {
		ret = freelist;
		freelist = freelist->next;
	} else {
		int sz = sizeof(*ret) + BUF_SIZE;
		ret = malloc(sz);
	}
	return ret;
}

void 
simp_release_allocator(struct simp_allocator* allocator) {
	if (!allocator) {
		return;
	}
	if (freelist) {
		allocator->next = freelist;
	} else {
		freelist = allocator;
		freelist->next = NULL;
	}
}
