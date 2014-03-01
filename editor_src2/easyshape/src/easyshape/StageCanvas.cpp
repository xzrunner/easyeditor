
#include "StageCanvas.h"
#include "StagePanel.h"
#include "Context.h"
#include "LibraryItem.h"

#include "../libshape/StageCanvas.h"

using namespace eshape;

StageCanvas::StageCanvas(StagePanel* editPanel)
	: d2d::ShapeStageCanvas(editPanel, editPanel, d2d::Colorf(1.0f, 0.0f, 0.0f))
{
}

void StageCanvas::onDraw()
{
	LibraryItem* item = Context::Instance()->stage->m_item;

	if (item) 
	{
		if (item->getFilepath().empty() || 
			d2d::FileNameParser::isType(item->getFilepath(), d2d::FileNameParser::e_shape)) {
			drawGuideLines();
		}
		else {
			item->draw();
		}
	}

	d2d::ShapeStageCanvas::onDraw();
}

void StageCanvas::drawGuideLines()
{
	d2d::PrimitiveDraw::rect(
		d2d::Vector(0, 0), 
		d2d::HALF_S_WIDTH,
		d2d::HALF_S_HEIGHT,
		d2d::LIGHT_GREY_LINE);
}