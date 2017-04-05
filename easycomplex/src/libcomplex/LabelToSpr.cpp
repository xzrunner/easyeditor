#include "LabelToSpr.h"

#include <easytext.h>

#include <sprite2/UpdateParams.h>

namespace ecomplex
{

ee::Sprite* LabelToSpr::Trans(const erespacker::PackLabel* label)
{
	etext::Symbol* sym = new etext::Symbol();

	etext::Sprite* spr = new etext::Sprite(sym);
	s2::Textbox& tb = spr->GetTextbox();

	tb.width		= label->width;
	tb.height		= label->height;

	tb.font_type	= label->font;
	tb.font_size	= label->font_size;
	tb.font_color	= label->font_color;

	tb.has_edge		= label->edge;
	tb.edge_size	= label->edge_size;
	tb.edge_color	= label->edge_color;

	tb.align_hori	= s2::Textbox::HoriAlign(label->align_hori);
	tb.align_vert	= s2::Textbox::VertAlign(label->align_vert);

	tb.space_hori	= label->space_hori;
	tb.space_vert	= label->space_vert;
	
	spr->SetText(s2::UpdateParams(), label->text);
	spr->SetTID(label->tid);

	return spr;
}

}