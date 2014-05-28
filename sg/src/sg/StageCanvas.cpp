#include "StageCanvas.h"
#include "StagePanel.h"

static const d2d::Colorf LIGHT_GRAY = d2d::Colorf(0.8f, 0.8f, 0.8f);

namespace sg
{

StageCanvas::StageCanvas(StagePanel* parent)
	: d2d::SpriteStageCanvas(parent, parent)
{
}

void StageCanvas::onDraw()
{
	drawGuideLines();
	d2d::SpriteStageCanvas::onDraw();
}

void StageCanvas::drawGuideLines()
{
	int row, col, edge;
	StagePanel* stage = static_cast<StagePanel*>(m_editPanel);
	stage->getLayoutInfo(row, col, edge);

	int width = col * edge;
	int height = row * edge;
	for (int i = 0; i <= row; ++i) {
		d2d::PrimitiveDraw::drawLine(d2d::Vector(0, i*edge), d2d::Vector(width, i*edge), LIGHT_GRAY);
	}
	for (int i = 0; i <= col; ++i) {
		d2d::PrimitiveDraw::drawLine(d2d::Vector(i*edge, 0), d2d::Vector(i*edge, height), LIGHT_GRAY);
	}
}

}
