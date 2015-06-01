#include "StageCanvas.h"
#include "StagePanel.h"

namespace etemplate
{

StageCanvas::StageCanvas(StagePanel* panel)
	: d2d::DynamicStageCanvas(panel)
	, m_panel(panel)
{
}

void StageCanvas::onDraw()
{
	m_panel->traverseSprites(d2d::DrawSpritesVisitor(), d2d::DT_VISIBLE);

	m_stage->drawEditTemp();
}

}