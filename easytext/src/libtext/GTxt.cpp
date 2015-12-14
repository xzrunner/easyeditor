#include "GTxt.h"
#include "Sprite.h"

#include <gtxt.h>
#include <rvg.h>

namespace etext
{

struct render_params
{
	const d2d::Matrix* mt;
	const d2d::Colorf* mul;
	const d2d::Colorf* add;
};

GTxt* GTxt::m_instance = NULL;

GTxt::GTxt()
{
	d2d::ShaderMgr::Instance()->sprite();
	Init();
}

void GTxt::LoadFont(const char* filepath)
{
}

void render_decoration(const d2d::Matrix& mat, float x, float y, float w, float h, float row_y, float row_h, struct gtxt_decoration* d)
{
	if (d->type == DT_NULL) {
		return;
	}

	d2d::ShaderMgr::Instance()->RVG();
	rvg_shader_color(d->color);

	float hw = w * 0.5f,
		  hh = h * 0.5f;
	if (d->type == DT_OVERLINE || d->type == DT_UNDERLINE || d->type == DT_STRIKETHROUGH) {
		d2d::Vector left(x - hw, y), right(x + hw, y);
		switch (d->type) 
		{
		case DT_OVERLINE:
			left.y = right.y = row_y + row_h;
			break;
		case DT_UNDERLINE:
			left.y = right.y = row_y;
			break;
		case DT_STRIKETHROUGH:
			left.y = right.y = row_y + row_h * 0.5f;
			break;
		}
		left = d2d::Math::transVector(left, mat);
		right = d2d::Math::transVector(right, mat);
		rvg_line(left.x, left.y, right.x, right.y);
	} else if (d->type == DT_BORDER || d->type == DT_BG) {
		
		float xmin = x - hw, xmax = x + hw;
		float ymin = row_y, ymax = row_y + row_h;
		rvg_rect(xmin, ymin, xmax, ymax, d->type == DT_BG);
	}
}

void render(int id, float* _texcoords, float x, float y, float w, float h, struct gtxt_draw_style* ds, void* ud) 
{
	render_params* rp = (render_params*)ud;

	x += ds->offset_x;
	y += ds->offset_y;
 	float hw = w * 0.5f * ds->scale, hh = h * 0.5f * ds->scale;

	d2d::Vector vertices[4];
	vertices[0] = d2d::Vector(x - hw, y + hh);
	vertices[1] = d2d::Vector(x - hw, y - hh);
	vertices[2] = d2d::Vector(x + hw, y - hh);
	vertices[3] = d2d::Vector(x + hw, y + hh);
	for (int i = 0; i < 4; ++i) {
		vertices[i] = d2d::Math::transVector(vertices[i], *rp->mt);
	}

	d2d::Vector texcoords[4];
	texcoords[0].set(_texcoords[0], _texcoords[1]);
	texcoords[1].set(_texcoords[2], _texcoords[3]);
	texcoords[2].set(_texcoords[4], _texcoords[5]);
	texcoords[3].set(_texcoords[6], _texcoords[7]);

	d2d::Colorf multi_col = *rp->mul;
	multi_col.a *= ds->alpha;

	d2d::ShaderMgr* mgr = d2d::ShaderMgr::Instance();
	mgr->sprite();
	mgr->SetSpriteColor(multi_col, *rp->add);
	mgr->Draw(vertices, texcoords, id);

	render_decoration(*rp->mt, x, y, w, h, ds->row_y, ds->row_h, &ds->decoration);
}

void*
ext_sym_create(const char* str) {
	if (strncmp(str, "path=", 5) != 0) {
		return NULL;
	}

	std::string filepath(&str[5]);
	d2d::ISymbol* sym = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
	return sym;
}

void
ext_sym_release(void* ext_sym) {
	if (!ext_sym) {
		return;
	}

// 	d2d::ISymbol* sym = (d2d::ISymbol*)ext_sym;
// 	sym->Release();
}

void 
ext_sym_get_size(void* ext_sym, int* width, int* height) {
	if (!ext_sym) {
		*width= *height = 0;
		return;
	}

	d2d::ISymbol* sym = (d2d::ISymbol*)ext_sym;
	d2d::Rect sz = sym->GetSize();
	*width = sz.xLength();
	*height = sz.yLength();
}

void
ext_sym_render(void* ext_sym, float x, float y, void* ud) {
	if (!ext_sym) {
		return;
	}

	d2d::ISymbol* sym = (d2d::ISymbol*)ext_sym;
	d2d::Matrix* mt = (d2d::Matrix*)ud;
	d2d::SpriteRenderer::Instance()->Draw(sym, *mt, d2d::Vector(x, y));
}

void GTxt::Draw(const Sprite* spr, const d2d::Matrix& mt,
				const d2d::Colorf& mul, const d2d::Colorf& add) const
{
	if (spr->GetText().empty()) {
		return;
	}

	gtxt_label_style style;

	spr->GetSize(style.width, style.height);
	spr->GetAlign(style.align_h, style.align_v);
	spr->GetSpace(style.space_h, style.space_v);

	style.gs.font = spr->GetFont();
	style.gs.font_size = spr->GetFontSize();
	style.gs.font_color.integer = d2d::trans_color2int(spr->GetFontColor(), d2d::PT_RGBA);

	style.gs.edge = spr->GetEdge();
	style.gs.edge_size = spr->GetEdgeSize();
	style.gs.edge_color.integer = d2d::trans_color2int(spr->GetEdgeColor(), d2d::PT_RGBA);

	render_params rp;
	rp.mt = &mt;
	rp.mul = &mul;
	rp.add = &add;

	std::string utf8 = d2d::StringTools::ToUtf8(spr->GetText());
	gtxt_label_draw_richtext(utf8.c_str(), &style, spr->GetTime(), render, (void*)&rp);

	spr->UpdateTime();
}

void GTxt::Draw(const d2d::Matrix& mt, const std::string& str) const
{
	if (str.empty()) {
		return;
	}

	gtxt_label_style style;

	style.width = 200;
	style.height = 50;

	style.align_h = HA_CENTER;
	style.align_v = VA_CENTER;

	style.space_h = style.space_v = 1;

	style.gs.font = 0;
	style.gs.font_size = 20;
	style.gs.font_color.integer = 0xffffffff;

	style.gs.edge = true;
	style.gs.edge_size = 1;
	style.gs.edge_color.integer = 0x000000ff;

	gtxt_label_draw(str.c_str(), &style, render, (void*)&mt);
}

void GTxt::Reload(const Sprite* spr) 
{
	if (spr->GetText().empty()) {
		return;
	}

	gtxt_label_style style;

	spr->GetSize(style.width, style.height);
	spr->GetAlign(style.align_h, style.align_v);
	spr->GetSpace(style.space_h, style.space_v);

	style.gs.font = spr->GetFont();
	style.gs.font_size = spr->GetFontSize();
	style.gs.font_color.integer = d2d::trans_color2int(spr->GetFontColor(), d2d::PT_RGBA);

	style.gs.edge = spr->GetEdge();
	style.gs.edge_size = spr->GetEdgeSize();
	style.gs.edge_color.integer = d2d::trans_color2int(spr->GetEdgeColor(), d2d::PT_RGBA);

	std::string utf8 = d2d::StringTools::ToUtf8(spr->GetText());
	gtxt_label_reload_richtext(utf8.c_str(), &style);
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
	dtex_cg* cg = d2d::DrawCallBatching::Instance()->GetDtexCG();
	gtxt_adapter_init(cg);

	gtxt_ft_init();

	const std::vector<std::pair<std::string, std::string> >& 
		fonts = d2d::Config::Instance()->GetFonts();
	for (int i = 0, n = fonts.size(); i < n; ++i) {
		gtxt_ft_add_font(fonts[i].first.c_str(), fonts[i].second.c_str());
	}	

	gtxt_glyph_init(50, 500, NULL);

	gtxt_richtext_ext_sym_cb_init(&ext_sym_create, &ext_sym_release, &ext_sym_get_size, &ext_sym_render, NULL);
}

}