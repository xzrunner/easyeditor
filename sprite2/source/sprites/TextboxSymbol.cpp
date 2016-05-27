#include "TextboxSymbol.h"

namespace s2
{

TextboxSymbol::TextboxSymbol(void* ud, Textbox tb)
	: Symbol(ud)
	, m_tb(tb)
{
	float hw = m_tb.width * 0.5f,
		  hh = m_tb.height * 0.5f;
	m_size.xmin = -hw; m_size.xmax = hw; 
	m_size.ymin = -hh; m_size.ymax = hh; 
}

void TextboxSymbol::Draw(const RenderParams& params, const Sprite* spr) const
{
// 	assert(spr);
// 	TextboxSprite* tb_spr = static_cast<TextboxSprite*>(spr);
// 	
// 	const std::string& text = tb_spr->GetText();
// 	if (text.empty()) {
// 		return;
// 	}
// 
// 	Textbox* tb = tb_spr->GetTextbox();
// 	if (!tb) {
// 		tb = &m_tb;
// 	}
// 
// 	gtxt_label_style s;
// 
// 	s.width		= tb->width;
// 	s.height	= tb->height;
// 
// 	s.gs.font	= font->GetFont();
// 	s.gs.font_size = font->GetFontSize();
// 	s.gs.font_color.integer = font->GetFontColor().ToRGBA();
// 
// 	s.gs.edge = font->GetEdge();
// 	s.gs.edge_size = font->GetEdgeSize();
// 	s.gs.edge_color.integer = font->GetEdgeColor().ToRGBA();
// 
// 	s.align_h	= tb->align_hori;
// 	s.align_v	= tb->align_vert;
// 
// 	s.space_h	= tb->space_hori;
// 	s.space_v	= tb->space_vert;
// 
// 	ee::GTxt::Instance()->Draw(s, params.mt, params.color.mul, params.color.add, font->GetText(), font->GetTime());
// 	font->UpdateTime();
}

}