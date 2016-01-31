#include "StageCanvas.h"
#include "StagePanel.h"
#include "LibraryItem.h"

namespace eshape
{

StageCanvas::StageCanvas(StagePanel* stage)
	: ee::ShapeStageCanvas(stage, stage, ee::Colorf(1.0f, 0.0f, 0.0f))
	, m_stage(stage)
{
}

void StageCanvas::onDraw()
{
	LibraryItem* item = m_stage->m_item;

	if (item) 
	{
		if (!item->getFilepath().empty() || 
			ee::FileNameParser::isType(item->getFilepath(), ee::FileNameParser::e_shape)) {
			drawGuideLines();
		}
		else {
			item->draw(ee::Matrix());
		}
	}

	ee::ShapeStageCanvas::onDraw();
}

void StageCanvas::drawGuideLines()
{
	ee::PrimitiveDraw::rect(ee::Vector(0, 0), 
		ee::HALF_S_WIDTH,
		ee::HALF_S_HEIGHT,
		ee::LIGHT_GREY_LINE);
}

}