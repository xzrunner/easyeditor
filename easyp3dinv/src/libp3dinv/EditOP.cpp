#include "EditOP.h"
#include "StagePanel.h"
#include "ParticleSystem.h"

namespace ep3dinv
{

EditOP::EditOP(StagePanel* stage)
	: d2d::ZoomViewOP(stage, stage->GetStageImpl(), true)
	, m_stage(stage)
{
}

bool EditOP::OnMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	ParticleSystem* ps = m_stage->m_ps;
	ps->Reset();

	return false;
}

}