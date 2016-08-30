#include "simp_texture.h"
#include "simp_import.h"

void 
simp_import_texture(struct simp_import_stream* is) {
	struct simp_texture* texture = simp_import_alloc(is, SIZEOF_TEXTURE);
	
	texture->type = simp_import_uint8(is);
}
