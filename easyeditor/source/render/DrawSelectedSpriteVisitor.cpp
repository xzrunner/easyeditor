#include "DrawSelectedSpriteVisitor.h"
#include "Vector.h"
#include "Sprite.h"
#include "BoundingBox.h"
#include "PrimitiveDraw.h"
#include "Config.h"
#include "SettingData.h"
#include "ImageSprite.h"
#include "Matrix.h"
#include "Image.h"
#include "style_config.h"

#include <vector>

namespace ee
{

DrawSelectedSpriteVisitor::DrawSelectedSpriteVisitor(const Colorf& color)
	: m_color(color)
{}

void DrawSelectedSpriteVisitor::Visit(Object* object, bool& next) 
{
	std::vector<Vector> bound;
	Sprite* sprite = static_cast<Sprite*>(object);
	sprite->GetBounding()->GetBoundPos(bound);

	PrimitiveDraw::DrawPolyline(bound, m_color, true);

	// todo: bad
	if (Config::Instance()->GetSettings().visible_image_edge)
	{
		if (ImageSprite* s = dynamic_cast<ImageSprite*>(sprite))
		{
			Matrix mt;
			s->GetTransMatrix(mt);

			Image* img = s->GetSymbol().GetImage();
			PrimitiveDraw::DrawRect(mt, img->GetOriginWidth() * 0.5f, 
				img->GetOriginHeight() * 0.5f, LIGHT_GREY_THIN_LINE);
		}
	}

	next = true;
}

}