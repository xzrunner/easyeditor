#include "SpriteTools.h"

#include "dataset/ISprite.h"
#include "common/Settings.h"
#include "common/Math.h"
#include "render/LabelNew.h"

namespace d2d
{

void SpriteTools::DrawName(const Screen& scr, const ISprite* sprite)
{
	if (!Settings::bVisibleNodeName) {
		return;
	}
	if (sprite && !sprite->name.empty() && sprite->name[0] != '_') {
		LabelStyle style;
		InitLabelStyle(style);

		LabelNew::Print(scr, sprite->name.c_str(), sprite->getPosition(), style);
	}
}

void SpriteTools::DrawName(const Screen& scr, const ISprite* sprite, const Matrix& mt)
{
 	if (!Settings::bVisibleNodeName) {
 		return;
 	}
 	if (sprite && !sprite->name.empty() && sprite->name[0] != '_') {
		Vector pos = sprite->getPosition();
		pos = Math::transVector(Vector(pos), mt);

		LabelStyle style;
		InitLabelStyle(style);

		LabelNew::Print(scr, sprite->name.c_str(), pos, style);
 	}
}

void SpriteTools::InitLabelStyle(LabelStyle& style)
{
	style.has_edge = false;
	style.font_size = 20;
	style.width = 200;
	style.height = 50;
	style.color = Colorf(0, 0, 0);
	style.align_hori = HAT_LEFT;
	style.align_vert = VAT_TOP;
}

}