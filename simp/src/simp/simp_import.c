#include "simp_import.h"
#include "simp_alloc.h"

#include <fault.h>

struct simp_import_stream {
	struct simp_allocator* alloc;
	struct simp_pack* pack;
	const char* stream;
	int size;
	int current_id;	// for debug
};

uint8_t 
simp_import_uint8(struct simp_import_stream* is) {
	if (is->size == 0) {
		fault("Invalid import stream (%d)", is->current_id);
	}
	uint8_t ret = (uint8_t)*(is->stream);
	++is->stream;
	--is->size;
	return ret;
}

uint16_t 
simp_import_uint16(struct simp_import_stream* is) {
	if (is->size < 2) {
		fault("Invalid import stream (%d)", is->current_id);
	}
	uint8_t low = (uint8_t)*(is->stream);
	uint8_t high = (uint8_t)*(is->stream + 1);
	is->stream += 2;
	is->size -= 2;
	return low | (uint32_t)high << 8;
}

uint32_t simp_import_uint32(struct simp_import_stream* is) {
	if (is->size < 4) {
		fault("Invalid import stream (%d)", is->current_id);
	}
	
	uint8_t b[4];
	b[0] = (uint8_t)*(is->stream);
	b[1] = (uint8_t)*(is->stream + 1);
	b[2] = (uint8_t)*(is->stream + 2);
	b[3] = (uint8_t)*(is->stream + 3);
	is->stream += 4;
	is->size -= 4;

	return b[0] | (uint32_t)b[1] << 8 | (uint32_t)b[2] << 16 | (uint32_t)b[3] << 24;
}

const char* simp_import_string(struct simp_import_stream* is) {
	int n = simp_import_uint8(is);
	if (n == 255) {
		return NULL;
	}
	if (is->size < n) {
		fault("Invalid stream (%d): read string failed", is->current_id);
	}
	char* buf = simp_import_alloc(is, (n + 3) & ~3);
	memcpy(buf, is->stream, n);
	buf[n] = 0;
	is->stream += n;
	is->size -= n;

	return buf;
}

uint32_t simp_import_rgba(struct simp_import_stream* is) {
	if (is->size < 4) {
		fault("Invalid import stream (%d)", is->current_id);
	}

	uint8_t b[4];
	b[0] = (uint8_t)*(is->stream);
	b[1] = (uint8_t)*(is->stream + 1);
	b[2] = (uint8_t)*(is->stream + 2);
	b[3] = (uint8_t)*(is->stream + 3);
	is->stream += 4;
	is->size -= 4;

	return b[0] | (uint32_t)b[1] << 8 | (uint32_t)b[2] << 16 | (uint32_t)b[3] << 24;
}

uint32_t simp_import_abgr(struct simp_import_stream* is) {
	if (is->size < 4) {
		fault("Invalid import stream (%d)", is->current_id);
	}

	uint8_t b[4];
	b[0] = (uint8_t)*(is->stream);
	b[1] = (uint8_t)*(is->stream + 1);
	b[2] = (uint8_t)*(is->stream + 2);
	b[3] = (uint8_t)*(is->stream + 3);
	is->stream += 4;
	is->size -= 4;

	return b[3] | (uint32_t)b[2] << 8 | (uint32_t)b[1] << 16 | (uint32_t)b[0] << 24;
}

void* 
simp_import_alloc(struct simp_import_stream* is, int sz) {
	void* ret = simp_alloc(is->alloc, sz);
	if (!ret) {
		is->alloc = simp_create_allocator();
		ret = simp_alloc(is->alloc, sz);
	}
	return ret;
}

struct simp_spr_pack* 
simp_import_pkg(void* data, int sz) {
// 	struct simp_allocator* alloc = simp_create_allocator();
// 	struct simp_spr_pack* head = simp_alloc(alloc, SIZEOF_PACK);
// 	head->
// 
// 	struct import_stream is;
// 	is.alloc = simp_create_allocator();
// 	is.pack = simp_alloc(is.alloc, SIZEOF_PACK);
// 	is.data = data;
// 	is.sz = sz;
// 
// 	while (is.sz != 0) {
// 		import_sprite(&is);
// 	}

	return NULL;
}