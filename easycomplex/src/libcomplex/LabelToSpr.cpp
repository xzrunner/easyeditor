#include "LabelToSpr.h"

namespace ecomplex
{

d2d::ISprite* LabelToSpr::Trans(const libcoco::PackLabel* label)
{
	d2d::FontBlankSymbol* symbol = new d2d::FontBlankSymbol();

	d2d::FontSprite* spr = new d2d::FontSprite(symbol);
	spr->font = label->font;
	spr->has_edge = label->has_edge;
	spr->color = label->color;
	spr->align_hori = label->align_hori;
	spr->align_vert = label->align_vert;
	spr->size = label->size;
	spr->width = label->width;
	spr->height = label->height;
	return spr;
}

}