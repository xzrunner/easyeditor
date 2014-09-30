#define NOMINMAX 
#include <gl/GLee.h>

#include "StageCanvas.h"
#include "StagePanel.h"


namespace libsketch
{

StageCanvas::StageCanvas(StagePanel* stage)
	: d2d::OrthoCanvas(stage)
	, m_stage(stage)
{
}

void StageCanvas::onDraw()
{
	d2d::PrimitiveDraw::cross(d2d::Vector(0, 0), 100, 100, d2d::Colorf(1, 0, 0));

 	std::vector<d2d::ISprite*> sprites;
 	m_stage->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
 
 	for (size_t i = 0, n = sprites.size(); i < n; ++i)
 	{
 		d2d::ISprite* sprite = sprites[i];
 		if (!sprite->visiable)
 			continue;
 		d2d::SpriteDraw::drawSprite(sprite);
 	}

	m_editPanel->drawEditTemp();
}

}