#include "gtxt_render.h"

#include <dtex_cg.h>

static struct dtex_cg* CG;

void 
gtxt_render_init(struct dtex_cg* cg) {
	CG = cg;
}

void
gtxt_draw_glyph(int unicode, struct gtxt_render_style* style, float x, float y) {
	// 	struct gtxt_glyph_layout layout;
	// 	uint8_t* buf = gtxt_glyph_get_bitmap(unicode, font, size, edge, &layout);

	int uid = 0;
	float* texcoords = dtex_cg_query(CG, unicode, style->color, style->size, style->font, style->edge, &uid);
	if (!texcoords) {
		struct gtxt_glyph_layout layout;
		uint8_t* buf = gtxt_glyph_get_bitmap(unicode, style->font, style->size, style->edge, &layout);



		dtex_cg_load();		
		return;
	}
	
	struct ej_vertex_pack vb[4];
	for (int i = 0; i < 4; ++i) {
		vb[i].tx = texcoords[i*2] * 0xffff;
		vb[i].ty = texcoords[i*2+1] * 0xffff;
	}

	ej_shader_texture(uid, 0);
	ej_shader_draw(vb, 0xffffffff, 0, 0xff0000ff, 0x00ff00ff, 0x0000ffff);
}
