#include "DrawSelectedSpriteVisitor.h"
#include "Sprite.h"
#include "Config.h"
#include "SettingData.h"
#include "ImageSprite.h"
#include "ImageSymbol.h"
#include "Image.h"
#include "color_config.h"

#include <sprite2/S2_RVG.h>
#include <sprite2/BoundingBox.h>
#include <SM_Calc.h>

#include <vector>

namespace ee
{

DrawSelectedSpriteVisitor::DrawSelectedSpriteVisitor(const s2::Color& color)
	: m_color(color)
{}

void DrawSelectedSpriteVisitor::Visit(Sprite* spr, bool& next) 
{
	std::vector<sm::vec2> bound;
	spr->GetBounding()->GetBoundPos(bound);
	s2::RVG::SetColor(m_color);
	s2::RVG::Polyline(bound, true);
	
	// todo: bad
	if (Config::Instance()->GetSettings().visible_image_edge)
	{
		if (ImageSprite* s = dynamic_cast<ImageSprite*>(spr))
		{
			s2::RVG::SetColor(LIGHT_GREY);
			s2::RVG::LineWidth(1);

			Image* img = dynamic_cast<ImageSymbol*>(s->GetSymbol())->GetImage();
			float hw = img->GetOriginSize().x * 0.5f,
				  hh = img->GetOriginSize().y * 0.5f;
			sm::mat4 mt = s->GetLocalMat();
			sm::vec2 min(-hw, -hh), max(hw, hh);
			min = mt * min;
			max = mt * max;

			s2::RVG::Rect(min, max, false);

			s2::RVG::LineWidth(2);
		}
	}

	next = true;
}

}