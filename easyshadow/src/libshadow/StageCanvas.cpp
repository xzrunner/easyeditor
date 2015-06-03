#include "StageCanvas.h"
#include "StagePanel.h"
#include "Symbol.h"

namespace eshadow
{

StageCanvas::StageCanvas(StagePanel* stage)
	: d2d::DynamicStageCanvas(stage)
	, m_stage(stage)
{
}

void StageCanvas::onDraw()
{
	m_stage->GetSymbol()->draw(d2d::Matrix());
	m_stage->drawEditTemp();
}

}