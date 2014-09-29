#include "StageCanvas.h"
#include "StagePanel.h"
#include "Context.h"
#include "config.h"

using namespace epacker;

StageCanvas::StageCanvas(StagePanel* stage)
	: d2d::SpriteStageCanvas(stage, stage)
{
}

void StageCanvas::onDraw()
{
	d2d::SpriteStageCanvas::onDraw();
	drawRegion();
}

void StageCanvas::drawRegion()
{
	const float width = Context::Instance()->width,
		height = Context::Instance()->height;

	// bg
	d2d::PrimitiveDraw::rect(
		d2d::Vector(0, 0), 
		d2d::Vector(width, height), 
		d2d::LIGHT_RED_LINE);

	int x = 0, y = 0;
	int count = static_cast<StagePanel*>(m_editPanel)->GetTextureAccount();
	for (int i = 0; i < count; ++i)
	{
		d2d::PrimitiveDraw::rect(d2d::Vector(x, y), 
			d2d::Vector(x + width, y + height), 
			d2d::LIGHT_GREY_LINE);
		x += Context::Instance()->width * TEXTURE_X_OFFSET_FACTOR;
	}
}