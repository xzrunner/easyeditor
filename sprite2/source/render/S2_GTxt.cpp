#include "S2_GTxt.h"

extern "C" {
	#include <dtex_facade.h>
	#include <gtxt.h>
}

namespace s2
{

SINGLETON_DEFINITION(GTxt)

GTxt::GTxt() 
{
//	sl::ShaderMgr::Instance()->SetShader(sl::SPRITE2);
	Init();
}

GTxt::~GTxt() 
{
}

void GTxt::Init()
{
// 	dtex_cg* cg = dtexf_get_cg();
// 	gtxt_adapter_create(cg);
// 
// 	gtxt_ft_create();
// 
// 	const std::vector<std::pair<std::string, std::string> >& 
// 		fonts = Config::Instance()->GetFonts();
// 	for (int i = 0, n = fonts.size(); i < n; ++i) {
// 		gtxt_ft_add_font(fonts[i].first.c_str(), fonts[i].second.c_str());
// 	}	
// 
// 	gtxt_glyph_create(50, 500, NULL);
// 
// 	gtxt_richtext_ext_sym_cb_init(&ext_sym_create, &ext_sym_release, &ext_sym_get_size, &ext_sym_render, NULL);
}

}