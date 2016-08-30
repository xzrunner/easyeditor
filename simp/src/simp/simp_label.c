#include "simp_label.h"
#include "simp_import.h"

void 
simp_import_label(struct simp_import_stream* is) {
	struct simp_label* label = simp_import_alloc(is, SIZEOF_LABEL);
	
	label->width = simp_import_uint16(is);
	label->height = simp_import_uint16(is);

	label->font = simp_import_uint8(is);
	label->font_size = simp_import_uint8(is);
	label->font_color = simp_import_rgba(is);

	label->edge = simp_import_uint8(is);
	label->edge_size = simp_import_uint16(is) / 1024.0f;
	label->edge_color = simp_import_rgba(is);

	label->align_hori = simp_import_uint8(is);
	label->align_vert = simp_import_uint8(is);

	label->space_hori = simp_import_uint16(is) / 1024.0f;
	label->space_vert = simp_import_uint16(is) / 1024.0f;

	uint8_t pack8 = simp_import_uint8(is);
	label->richtext = pack8 & 0x1;
	label->overflow = pack8 & 0x2;

	label->text = simp_import_string(is);
	label->tid = simp_import_string(is);
}