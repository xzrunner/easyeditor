#include "DrawSelectedSpriteVisitor.h"
#include "Vector.h"
#include "Sprite.h"
#include "BoundingBox.h"
#include "EE_RVG.h"
#include "Config.h"
#include "SettingData.h"
#include "ImageSprite.h"
#include "Matrix.h"
#include "Image.h"
#include "style_config.h"
#include "Math2D.h"

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

	RVG::Polyline(bound, m_color, true);

	// todo: bad
	if (Config::Instance()->GetSettings().visible_image_edge)
	{
		if (ImageSprite* s = dynamic_cast<ImageSprite*>(sprite))
		{
			Matrix mt;
			s->GetTransMatrix(mt);

			Image* img = s->GetSymbol().GetImage();
			int hw = Math2D::TransLen(img->GetOriginWidth() * 0.5f, mt),
				hh = Math2D::TransLen(img->GetOriginHeight() * 0.5f, mt);
			RVG::Rect(ee::Vector(0, 0), hw, hh, LIGHT_GREY_THIN_LINE);
		}
	}

	next = true;
}

}