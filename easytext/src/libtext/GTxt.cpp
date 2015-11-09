#include "GTxt.h"
#include "Sprite.h"

#include <dtex.h>
#include <gtxt.h>

namespace etext
{

GTxt* GTxt::m_instance = NULL;

static const char* CFG =
	"{ \n"
	"	\"open_c1\" : false, \n"
	"	\"open_c2\" : true, \n"
	"	\"open_c3\" : false,	 \n"
	"	\"open_cg\" : true,	 \n"
	"	\"c2_tex_size\" : 512	 \n"
	"} \n"
	;

GTxt::GTxt()
{
	Init();
}

void GTxt::LoadFont(const char* filepath)
{
}

void render(int id, float* _texcoords, float x, float y, float w, float h, void* ud) 
{
	d2d::Matrix* mt = (d2d::Matrix*)ud;

	float hw = w * 0.5f, hh = h * 0.5f;

	d2d::Vector vertices[4];
	vertices[0] = d2d::Vector(x - hw, y + hh);
	vertices[1] = d2d::Vector(x - hw, y - hh);
	vertices[2] = d2d::Vector(x + hw, y - hh);
	vertices[3] = d2d::Vector(x + hw, y + hh);
	for (int i = 0; i < 4; ++i) {
		vertices[i] = d2d::Math::transVector(vertices[i], *mt);
	}

	d2d::Vector texcoords[4];
	texcoords[0].set(_texcoords[0], _texcoords[1]);
	texcoords[1].set(_texcoords[2], _texcoords[3]);
	texcoords[2].set(_texcoords[4], _texcoords[5]);
	texcoords[3].set(_texcoords[6], _texcoords[7]);

	d2d::ShaderMgr* mgr = d2d::ShaderMgr::Instance();
	mgr->sprite();
	mgr->Draw(vertices, texcoords, id);
}

void GTxt::Draw(const d2d::Matrix& mt, const Sprite* spr) const
{
	dtex_shader_texture(0);
	d2d::ShaderMgr::Instance()->SetTexture(0);

	gtxt_label_style style;

	style.font = spr->GetFont();
	style.font_size = spr->GetFontSize();
	style.edge = spr->GetEdge();

	spr->GetSize(style.width, style.height);

	style.color = d2d::trans_color2int(spr->GetColor(), d2d::PT_RGBA);
	
	spr->GetAlign(style.align_h, style.align_v);

	style.space_h = style.space_v = 0;

	std::string utf8 = d2d::StringTools::ToUtf8(spr->GetText());
	gtxt_label_draw(utf8.c_str(), &style, render, (void*)&mt);
}

void GTxt::ReloadTexture()
{
	dtex_shader_load();

// 	dtex_cg* old_cg = dtexf_get_cg();
// 	dtex_cg_clear(old_cg);

	dtexf_release();
	dtexf_create(CFG);

	dtex_cg* cg = dtexf_get_cg();
	gtxt_render_init(cg);
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

	dtexf_create(CFG);

	dtex_cg* cg = dtexf_get_cg();
	gtxt_render_init(cg);

	gtxt_ft_init();
//	gtxt_ft_add_font("FZCY_GBK.ttf");
//	gtxt_ft_add_font("msyh.ttf");

	const std::vector<std::pair<std::string, std::string> >& 
		fonts = d2d::Config::Instance()->GetFonts();
	for (int i = 0, n = fonts.size(); i < n; ++i) {
		gtxt_ft_add_font(fonts[i].second.c_str());
	}	

	gtxt_glyph_cache_init(50, 500);
}

}