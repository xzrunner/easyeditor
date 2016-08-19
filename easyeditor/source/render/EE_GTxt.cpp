#include "EE_GTxt.h"
#include "Symbol.h"
#include "ImageSymbol.h"
#include "SymbolMgr.h"
#include "SpriteRenderer.h"
#include "EE_DTex.h"
#include "Config.h"
#include "StringHelper.h"

#include <gtxt.h>
#include <sprite2/S2_GTxt.h>
#include <sprite2/RenderParams.h>
#include <sprite2/NodeRenderer.h>

#include <json/json.h>

#include <fstream>

namespace ee
{

GTxt* GTxt::m_instance = NULL;

GTxt::GTxt()
{
	Init();
}

void GTxt::LoadFont(const std::string& name, const std::string& filepath)
{
	s2::GTxt::Instance()->LoadFont(name, filepath);
}

void GTxt::LoadUserFont(const std::string& name, const std::string& filepath)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	int sz = value["chars"].size();

	int font = gtxt_uf_add_font(name.c_str(), sz + 1);

	int idx = 0;
	Json::Value c_val = value["chars"][idx++];
	while (!c_val.isNull()) {
		std::string str = c_val["str"].asString();
		std::string utf8 = StringHelper::ToUtf8(str);
		std::string filepath = c_val["filepath"].asString();
		Symbol* sym = SymbolMgr::Instance()->FetchSymbol(filepath);
		sm::vec2 sz = sym->GetBounding().Size();
		gtxt_uf_add_char(font, utf8.c_str(), (int)sz.x, (int)sz.y, sym);
		c_val = value["chars"][idx++];
	}
}

void GTxt::Draw(const gtxt_label_style& style, const sm::mat4& mt, const s2::Color& mul, 
				const s2::Color& add, const std::string& text, int time, bool richtext) const
{
	s2::GTxt::Instance()->Draw(style, mt, mul, add, text, time, richtext);
}

void GTxt::Draw(const sm::mat4& mt, const std::string& str) const
{
	s2::GTxt::Instance()->Draw(mt, str);
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
// 	sym->RemoveReference();
}

void 
ext_sym_get_size(void* ext_sym, int* width, int* height) {
	if (!ext_sym) {
		*width= *height = 0;
		return;
	}

	Symbol* sym = (Symbol*)ext_sym;
	sm::vec2 sz = sym->GetBounding().Size();
	*width  = static_cast<int>(sz.x);
	*height = static_cast<int>(sz.y);
}

void
ext_sym_render(void* ext_sym, float x, float y, void* ud) {
	if (!ext_sym) {
		return;
	}
	s2::RenderParams params;
	params.mt = *((sm::mat4*)ud);
	s2::NodeRenderer::Draw((Symbol*)ext_sym, params, sm::vec2(x, y));
}

float* 
uf_query_and_load(void* ud, struct dtex_glyph* glyph) {
	DTex* dtex = DTex::Instance();
	dtex_cg* cg = dtex->GetDtexCG();
	ImageSymbol* sym = (ImageSymbol*)ud;
	int texid = 0;
	return dtex->Query(sym->GetImage(), &texid);
//	sm::vec2 sz = sym->GetSize().Size();
//	int texid = sym->GetImage()->GetTexID();
//	float* texcoords = dtex_cg_load_tex(cg, texid, (int)sz.x, (int)sz.y, glyph);
// 	if (texcoords) {
// 		return texcoords;
// 	} else {
// 		dtex->LoadBegin();
// 		dtex->Load(sym->GetImage());
// 		dtex->LoadEnd();
// 		return NULL;
// 	}
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
	s2::GTxt* gtxt = s2::GTxt::Instance();

	dtex_cg* cg = DTex::Instance()->GetDtexCG();
	gtxt_adapter_create(cg);

	gtxt_ft_create();

	const std::vector<std::pair<std::string, std::string> >& 
		fonts = Config::Instance()->GetFonts();
	for (int i = 0, n = fonts.size(); i < n; ++i) {
		gtxt->LoadFont(fonts[i].first.c_str(), fonts[i].second.c_str());
	}

	gtxt_glyph_create(50, 500, NULL);

	gtxt_richtext_ext_sym_cb_init(&ext_sym_create, &ext_sym_release, &ext_sym_get_size, &ext_sym_render, NULL);

	gtxt_uf_cb_init(uf_query_and_load);
	gtxt_uf_create();
	const std::vector<std::pair<std::string, std::string> >& 
		user_fonts = Config::Instance()->GetUserFonts();
	for (int i = 0, n = user_fonts.size(); i < n; ++i) {
		LoadUserFont(user_fonts[i].first, user_fonts[i].second);
	}
}

}