#include "DrawSelectedSpriteVisitor.h"
#include "PrimitiveDraw.h"

#include "dataset/AbstractBV.h"
#include "dataset/ISprite.h"
#include "dataset/ImageSprite.h"
#include "dataset/Image.h"
#include "render/style_config.h"
#include "render/GL10.h"
#include "common/Matrix.h"
#include "common/Config.h"
#include "common/SettingData.h"

namespace d2d
{

DrawSelectedSpriteVisitor::DrawSelectedSpriteVisitor(const Colorf& color)
	: m_color(color)
{}

void DrawSelectedSpriteVisitor::Visit(Object* object, bool& bFetchNext) 
{
	std::vector<Vector> bound;
	ISprite* sprite = static_cast<ISprite*>(object);
	sprite->GetBounding()->getBoundPos(bound);

	PrimitiveDraw::drawPolyline(bound, m_color, true);

	// todo: bad
	if (Config::Instance()->GetSettings().visible_image_edge)
	{
		if (ImageSprite* s = dynamic_cast<ImageSprite*>(sprite))
		{
			Matrix mt;
			s->GetTransMatrix(mt);

			Image* img = s->GetSymbol().getImage();
			PrimitiveDraw::rect(mt, img->GetOriginWidth() * 0.5f, 
				img->GetOriginHeight() * 0.5f, LIGHT_GREY_THIN_LINE);
		}
	}

	bFetchNext = true;
}

} // d2d