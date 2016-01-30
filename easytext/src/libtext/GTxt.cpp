#include "GTxt.h"
#include "Sprite.h"

#include <gtxt.h>

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

static void
render_glyph(int id, float* _texcoords, float x, float y, float w, float h, struct gtxt_draw_style* ds, render_params* rp) 
{
	x += ds->offset_x;
	y += ds->offset_y;
	float hw = w * 0.5f * ds->scale, hh = h * 0.5f * ds->scale;

	d2d::Vector vertices[4];
	vertices[0] = d2d::Vector(x - hw, y + hh);
	vertices[1] = d2d::Vector(x - hw, y - hh);
	vertices[2] = d2d::Vector(x + hw, y - hh);
	vertices[3] = d2d::Vector(x + hw, y + hh);
	for (int i = 0; i < 4; ++i) {
		vertices[i] = d2d::Math2D::TransVector(vertices[i], *rp->mt);
	}

	d2d::Vector texcoords[4];
	texcoords[0].Set(_texcoords[0], _texcoords[1]);
	texcoords[1].Set(_texcoords[2], _texcoords[3]);
	texcoords[2].Set(_texcoords[4], _texcoords[5]);
	texcoords[3].Set(_texcoords[6], _texcoords[7]);

	d2d::ColorTrans color;
	if (rp->mul) {
		d2d::Colorf multi_col = *rp->mul;
		multi_col.a *= ds->alpha;
		color.multi = multi_col;
	} 
	if (rp->add) {
		color.add = *rp->add;
	}

	d2d::ShaderMgr* mgr = d2d::ShaderMgr::Instance();
	mgr->sprite();
	mgr->SetSpriteColor(color);
	mgr->Draw(vertices, texcoords, id);
}

static void 
render_decoration(const d2d::Matrix& mat, float x, float y, float w, float h, struct gtxt_draw_style* ds)
{
	struct gtxt_decoration* d = &ds->decoration;
	if (d->type == GRDT_NULL) {
		return;
	}

	d2d::ShaderMgr::Instance()->RVG();
	d2d::PrimitiveDrawRVG::SetColor(d->color);

	float hw = w * 0.5f,
		  hh = h * 0.5f;
	if (d->type == GRDT_OVERLINE || d->type == GRDT_UNDERLINE || d->type == GRDT_STRIKETHROUGH) {
		d2d::Vector left(x - hw, y), right(x + hw, y);
		switch (d->type) 
		{
		case GRDT_OVERLINE:
			left.y = right.y = ds->row_y + ds->row_h;
			break;
		case GRDT_UNDERLINE:
			left.y = right.y = ds->row_y;
			break;
		case GRDT_STRIKETHROUGH:
			left.y = right.y = ds->row_y + ds->row_h * 0.5f;
			break;
		}
		d2d::PrimitiveDrawRVG::Line(mat, left, right);
	} else if (d->type == GRDT_BORDER || d->type == GRDT_BG) {
		d2d::Vector min(x - hw, ds->row_y),
			        max(x + hw, ds->row_y + ds->row_h);
		if (d->type == GRDT_BG) {
			d2d::PrimitiveDrawRVG::Rect(mat, min, max, true);
		} else if (ds->pos_type != GRPT_NULL) {
			d2d::PrimitiveDrawRVG::Line(mat, min, d2d::Vector(max.x, min.y));
			d2d::PrimitiveDrawRVG::Line(mat, d2d::Vector(min.x, max.y), max);
			if (ds->pos_type == GRPT_BEGIN) {
				d2d::PrimitiveDrawRVG::Line(mat, min, d2d::Vector(min.x, max.y));
			}
			if (ds->pos_type == GRPT_END) {
				d2d::PrimitiveDrawRVG::Line(mat, d2d::Vector(max.x, min.y), max);
			}
		}
	}
}

static void 
render(int id, float* _texcoords, float x, float y, float w, float h, struct gtxt_draw_style* ds, void* ud) 
{
 	render_params* rp = (render_params*)ud;
	if (ds) {
		if (ds->decoration.type == GRDT_BG) {
			render_decoration(*rp->mt, x, y, w, h, ds);
			render_glyph(id, _texcoords, x, y, w, h, ds, rp);
		} else {
			render_glyph(id, _texcoords, x, y, w, h, ds, rp);
			render_decoration(*rp->mt, x, y, w, h, ds);
		}
	} else {
		struct gtxt_draw_style ds;
		ds.alpha = 1;
		ds.scale = 1;
		ds.offset_x = ds.offset_y = 0;
		render_glyph(id, _texcoords, x, y, w, h, &ds, rp);
	}
}

void*
ext_sym_create(const char* str) {
	if (strncmp(str, "path=", 5) != 0) {
		return NULL;
	}

	std::string filepath(&str[5]);
	d2d::Symbol* sym = d2d::SymbolMgr::Instance()->FetchSymbol(filepath);
	return sym;
}

void
ext_sym_release(void* ext_sym) {
	if (!ext_sym) {
		return;
	}

// 	d2d::Symbol* sym = (d2d::Symbol*)ext_sym;
// 	sym->Release();
}

void 
ext_sym_get_size(void* ext_sym, int* width, int* height) {
	if (!ext_sym) {
		*width= *height = 0;
		return;
	}

	d2d::Symbol* sym = (d2d::Symbol*)ext_sym;
	d2d::Rect sz = sym->GetSize();
	*width = sz.Width();
	*height = sz.Height();
}

void
ext_sym_render(void* ext_sym, float x, float y, void* ud) {
	if (!ext_sym) {
		return;
	}

	d2d::Symbol* sym = (d2d::Symbol*)ext_sym;
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
	style.gs.font_color.integer = d2d::color2int(spr->GetFontColor(), d2d::PT_RGBA);

	style.gs.edge = spr->GetEdge();
	style.gs.edge_size = spr->GetEdgeSize();
	style.gs.edge_color.integer = d2d::color2int(spr->GetEdgeColor(), d2d::PT_RGBA);

	render_params rp;
	rp.mt = &mt;
	rp.mul = &mul;
	rp.add = &add;

	std::string utf8 = d2d::StringHelper::ToUtf8(spr->GetText());
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

	render_params rp;
	rp.mt = &mt;
	rp.mul = NULL;
	rp.add = NULL;

	gtxt_label_draw(str.c_str(), &style, render, (void*)&rp);
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
	style.gs.font_color.integer = d2d::color2int(spr->GetFontColor(), d2d::PT_RGBA);

	style.gs.edge = spr->GetEdge();
	style.gs.edge_size = spr->GetEdgeSize();
	style.gs.edge_color.integer = d2d::color2int(spr->GetEdgeColor(), d2d::PT_RGBA);

	std::string utf8 = d2d::StringHelper::ToUtf8(spr->GetText());
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
	gtxt_adapter_create(cg);

	gtxt_ft_create();

	const std::vector<std::pair<std::string, std::string> >& 
		fonts = d2d::Config::Instance()->GetFonts();
	for (int i = 0, n = fonts.size(); i < n; ++i) {
		gtxt_ft_add_font(fonts[i].first.c_str(), fonts[i].second.c_str());
	}	

	gtxt_glyph_create(50, 500, NULL);

	gtxt_richtext_ext_sym_cb_init(&ext_sym_create, &ext_sym_release, &ext_sym_get_size, &ext_sym_render, NULL);
}

}