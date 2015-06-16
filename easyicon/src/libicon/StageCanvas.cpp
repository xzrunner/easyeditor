#include "StageCanvas.h"
#include "StagePanel.h"
#include "Sprite.h"

namespace eicon
{

StageCanvas::StageCanvas(StagePanel* stage)
	: d2d::DynamicStageCanvas(stage)
	, m_stage(stage)
{
}

void StageCanvas::OnDraw()
{
	Sprite sprite;
	sprite.setSymbol(&m_stage->GetSymbol());
	sprite.SetProcess(0.5f);

	m_stage->GetSymbol().draw(d2d::Matrix(), d2d::Colorf(1, 1, 1, 1), d2d::Colorf(0, 0, 0, 0),
		d2d::Colorf(1, 0, 0, 0), d2d::Colorf(0, 1, 0, 0), d2d::Colorf(0, 0, 1, 0), &sprite);

	m_stage->drawEditTemp();
}

}