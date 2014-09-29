#include "StageCanvas.h"
#include "StagePanel.h"
#include "LibraryItem.h"

namespace eshape
{

StageCanvas::StageCanvas(StagePanel* stage)
	: d2d::ShapeStageCanvas(stage, stage, d2d::Colorf(1.0f, 0.0f, 0.0f))
	, m_stage(stage)
{
}

void StageCanvas::onDraw()
{
	LibraryItem* item = m_stage->m_item;

	if (item) 
	{
		if (!item->getFilepath().empty() || 
			d2d::FileNameParser::isType(item->getFilepath(), d2d::FileNameParser::e_shape)) {
			drawGuideLines();
		}
		else {
			item->draw(d2d::Matrix());
		}
	}

	d2d::ShapeStageCanvas::onDraw();
}

void StageCanvas::drawGuideLines()
{
	d2d::PrimitiveDraw::rect(d2d::Vector(0, 0), 
		d2d::HALF_S_WIDTH,
		d2d::HALF_S_HEIGHT,
		d2d::LIGHT_GREY_LINE);
}

}