#include "StageCanvas.h"
#include "StagePanel.h"
#include "Sprite.h"

namespace eicon
{

StageCanvas::StageCanvas(StagePanel* stage)
	: d2d::OrthoCanvas(stage, stage->GetStageImpl())
	, m_stage(stage)
{
}

void StageCanvas::OnDrawSprites() const
{
	Sprite sprite;
	sprite.SetSymbol(&m_stage->GetSymbol());
	sprite.SetProcess(0.5f);

	m_stage->GetSymbol().Draw(d2d::Matrix(), d2d::Colorf(1, 1, 1, 1), d2d::Colorf(0, 0, 0, 0),
		d2d::Colorf(1, 0, 0, 0), d2d::Colorf(0, 1, 0, 0), d2d::Colorf(0, 0, 1, 0), &sprite);

	d2d::PrimitiveDraw::cross(d2d::Vector(0, 0), 100, 100, d2d::Colorf(1, 0, 0));;

	m_stage->DrawEditOP();
}

}