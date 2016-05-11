#include "EE_GTxt.h"
#include "Math2D.h"
#include "Symbol.h"
#include "SymbolMgr.h"
#include "SpriteRenderer.h"
#include "EE_DTex.h"
#include "Config.h"
#include "StringHelper.h"
#include "EE_RVG.h"

#include <gtxt.h>
#include <shaderlab.h>

namespace ee
{

struct render_params
{
	const sm::mat4* mt;
	const s2::Color* mul;
	const s2::Color* add;
};

GTxt* GTxt::m_instance = NULL;

GTxt::GTxt()
{
	sl::ShaderMgr::Instance()->SetShader(sl::SPRITE2);
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

	sm::vec2 vertices[4];
	vertices[0] = sm::vec2(x - hw, y + hh);
	vertices[1] = sm::vec2(x - hw, y - hh);
	vertices[2] = sm::vec2(x + hw, y - hh);
	vertices[3] = sm::vec2(x + hw, y + hh);
	for (int i = 0; i < 4; ++i) {
		vertices[i] = Math2D::TransVector(vertices[i], *rp->mt);
	}

	sm::vec2 texcoords[4];
	texcoords[0].Set(_texcoords[0], _texcoords[1]);
	texcoords[1].Set(_texcoords[2], _texcoords[3]);
	texcoords[2].Set(_texcoords[4], _texcoords[5]);
	texcoords[3].Set(_texcoords[6], _texcoords[7]);

	s2::RenderColor color;
	if (rp->mul) {
		s2::Color multi_col = *rp->mul;
		multi_col.a = static_cast<int>(multi_col.a * ds->alpha);
		color.mul = multi_col;
	} 
	if (rp->add) {
		color.add = *rp->add;
	}

	sl::ShaderMgr* sl_mgr = sl::ShaderMgr::Instance();
	sl_mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* sl_shader = static_cast<sl::Sprite2Shader*>(sl_mgr->GetShader());
	sl_shader->SetColor(color.mul.ToABGR(), color.add.ToABGR());
	sl_shader->SetColorMap(color.rmap.ToABGR(), color.gmap.ToABGR(), color.bmap.ToABGR());
	sl_shader->Draw(&vertices[0].x, &texcoords[0].x, id);
}

static void 
render_decoration(const sm::mat4& mat, float x, float y, float w, float h, struct gtxt_draw_style* ds)
{
	struct gtxt_decoration* d = &ds->decoration;
	if (d->type == GRDT_NULL) {
		return;
	}

	sl::ShaderMgr* sl_mgr = sl::ShaderMgr::Instance();
	sl_mgr->SetShader(sl::SHAPE2);
	sl::Shape2Shader* sl_shader = static_cast<sl::Shape2Shader*>(sl_mgr->GetShader());
	sl_shader->SetColor(d->color);

	float hw = w * 0.5f,
		  hh = h * 0.5f;
	if (d->type == GRDT_OVERLINE || d->type == GRDT_UNDERLINE || d->type == GRDT_STRIKETHROUGH) {
		sm::vec2 left(x - hw, y), right(x + hw, y);
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
		RVG::Line(Math2D::TransVector(left, mat), Math2D::TransVector(right, mat));
	} else if (d->type == GRDT_BORDER || d->type == GRDT_BG) {
		sm::vec2 min(x - hw, ds->row_y), 
			   max(x + hw, ds->row_y + ds->row_h);
		min = Math2D::TransVector(min, mat);
		max = Math2D::TransVector(max, mat);
		if (d->type == GRDT_BG) {
			RVG::Rect(min, max, true);
		} else if (ds->pos_type != GRPT_NULL) {
			RVG::Line(min, sm::vec2(max.x, min.y));
			RVG::Line(sm::vec2(min.x, max.y), max);
			if (ds->pos_type == GRPT_BEGIN) {
				RVG::Line(min, sm::vec2(min.x, max.y));
			}
			if (ds->pos_type == GRPT_END) {
				RVG::Line(sm::vec2(max.x, min.y), max);
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
	Symbol* sym = SymbolMgr::Instance()->FetchSymbol(filepath);
	return sym;
}

void
ext_sym_release(void* ext_sym) {
	if (!ext_sym) {
		return;
	}

// 	Symbol* sym = (Symbol*)ext_sym;
// 	sym->Release();
}

void 
ext_sym_get_size(void* ext_sym, int* width, int* height) {
	if (!ext_sym) {
		*width= *height = 0;
		return;
	}

	Symbol* sym = (Symbol*)ext_sym;
	Rect sz = sym->GetSize();
	*width  = static_cast<int>(sz.Width());
	*height = static_cast<int>(sz.Height());
}

void
ext_sym_render(void* ext_sym, float x, float y, void* ud) {
	if (!ext_sym) {
		return;
	}

	SpriteRenderer::Draw((Symbol*)ext_sym, 
		RenderParams(*((sm::mat4*)ud)), sm::vec2(x, y));
}

void GTxt::Draw(const gtxt_label_style& style, const sm::mat4& mt, const s2::Color& mul, 
				const s2::Color& add, const std::string& text, int time) const
{
	render_params rp;
	rp.mt = &mt;
	rp.mul = &mul;
	rp.add = &add;

	std::string utf8 = StringHelper::ToUtf8(text);
	gtxt_label_draw_richtext(utf8.c_str(), &style, time, render, (void*)&rp);
}

void GTxt::Draw(const sm::mat4& mt, const std::string& str) const
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

//void GTxt::Reload(const Sprite* spr) 
//{
//	if (spr->GetText().empty()) {
//		return;
//	}
//
//	gtxt_label_style style;
//
//	spr->GetSize(style.width, style.height);
//	spr->GetAlign(style.align_h, style.align_v);
//	spr->GetSpace(style.space_h, style.space_v);
//
//	style.gs.font = spr->GetFont();
//	style.gs.font_size = spr->GetFontSize();
//	style.gs.font_color.integer = color2int(spr->GetFontColor(), PT_RGBA);
//
//	style.gs.edge = spr->GetEdge();
//	style.gs.edge_size = spr->GetEdgeSize();
//	style.gs.edge_color.integer = color2int(spr->GetEdgeColor(), PT_RGBA);
//
//	std::string utf8 = StringHelper::ToUtf8(spr->GetText());
//	gtxt_label_reload_richtext(utf8.c_str(), &style);
//}

GTxt* GTxt::Instance()
{
	if (!m_instance) {
		m_instance = new GTxt();
	}
	return m_instance;
}

void GTxt::Init()
{
	dtex_cg* cg = DTex::Instance()->GetDtexCG();
	gtxt_adapter_create(cg);

	gtxt_ft_create();

	const std::vector<std::pair<std::string, std::string> >& 
		fonts = Config::Instance()->GetFonts();
	for (int i = 0, n = fonts.size(); i < n; ++i) {
		gtxt_ft_add_font(fonts[i].first.c_str(), fonts[i].second.c_str());
	}	

	gtxt_glyph_create(50, 500, NULL);

	gtxt_richtext_ext_sym_cb_init(&ext_sym_create, &ext_sym_release, &ext_sym_get_size, &ext_sym_render, NULL);
}

}