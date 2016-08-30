#include "simp_picture.h"
#include "simp_import.h"

void 
simp_import_picture(struct simp_import_stream* is) {
	struct simp_picture* pic = simp_import_alloc(is, SIZEOF_PICTURE);
	pic->texid = simp_import_uint8(is);
	for (int i = 0; i < 8; ++i) {
		pic->texcoords[i] = simp_import_uint16(is);
	}
}