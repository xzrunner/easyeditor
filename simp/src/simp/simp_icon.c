#include "simp_icon.h"
#include "simp_import.h"

void 
simp_import_icon(struct simp_import_stream* is) {
	struct simp_icon* icon = simp_import_alloc(is, SIZEOF_ICON);
	
	icon->type = simp_import_uint8(is);
}

void 
simp_import_icon_spr(struct simp_import_stream* is) {
	struct simp_icon_spr* icon = simp_import_alloc(is, SIZEOF_ICON_SPR);
	icon->sym = simp_import_uint32(is);
	icon->process = simp_import_uint16(is);
}