#include "GTxt.h"

#include <dtex.h>
#include <gtxt.h>

namespace etext
{

GTxt* GTxt::m_instance = NULL;

GTxt::GTxt()
{
	Init();
}

void GTxt::LoadFont(const char* filepath)
{
}

void GTxt::Draw(const char* str) const
{
	gtxt_label_style style;

	style.font = 0;
	style.font_size = 30;
	style.edge = false;

	style.width = 200;
	style.height = 200;

	style.color = 0x000000ff;

	style.align_h = HA_LEFT;
	style.align_v = VA_TOP;

	style.space_h = style.space_v = 0;

	gtxt_label_draw(str, &style);
}

GTxt* GTxt::Instance()
{
	if (!m_instance) {
		m_instance = new GTxt();
	}
	return m_instance;
}

void GTxt::Init()
{
	dtex_shader_load();

	const char* cfg =
		"{ \n"
		"	\"open_c1\" : false, \n"
		"	\"open_c2\" : true, \n"
		"	\"open_c3\" : false,	 \n"
		"	\"open_cg\" : true,	 \n"
		"	\"c2_tex_size\" : 512	 \n"
		"} \n"
		;
	dtexf_create(cfg);

	dtex_cg* cg = dtexf_get_cg();
	gtxt_render_init(cg);

	gtxt_ft_init();
	gtxt_ft_add_font("FZCY_GBK.ttf");

	gtxt_glyph_cache_init(50, 500);
}

}