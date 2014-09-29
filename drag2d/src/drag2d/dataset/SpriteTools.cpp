#include "SpriteTools.h"

#include "dataset/ISprite.h"
#include "common/Config.h"
#include "common/SettingData.h"
#include "common/Math.h"
#include "render/LabelNew.h"

namespace d2d
{

void SpriteTools::DrawName(const ISprite* sprite, const Matrix& mt)
{
	if (!Config::Instance()->GetSettings().visible_node_name) {
 		return;
 	}
 	if (sprite && !sprite->name.empty() && sprite->name[0] != '_') {
		Vector pos = Math::transVector(Vector(0, 0), mt);

		LabelStyle style;
		style.has_edge = false;
		style.font_size = 20;
		style.width = 200;
		style.height = 50;
		style.color = Colorf(0, 0, 0);
		style.align_hori = HAT_CENTER;
		style.align_vert = VAT_TOP;

		LabelNew::Print(sprite->name.c_str(), pos, style);
 	}
}

}