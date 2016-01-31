#include "LabelToSpr.h"

#include <easytext.h>

namespace ecomplex
{

ee::Sprite* LabelToSpr::Trans(const librespacker::PackLabel* label)
{
	etext::Symbol* symbol = new etext::Symbol();

	etext::Sprite* spr = new etext::Sprite(symbol);

	spr->SetSize(label->width, label->height);

	spr->SetFont(label->font);
	spr->SetFontSize(label->font_size);
	spr->SetFontColor(label->font_color);
	
	spr->SetEdge(label->edge);
	spr->SetEdgeSize(label->edge_size);
	spr->SetEdgeColor(label->edge_color);

	spr->SetAlign(label->align_hori, label->align_vert);
	
	spr->SetSpace(label->space_hori, label->space_vert);

	spr->SetText(label->text);
	spr->SetTID(label->tid);

	return spr;
}

}