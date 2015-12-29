#include "EditOP.h"
#include "StagePanel.h"
#include "ParticleSystem.h"

namespace eparticle3d
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
	m_stage->SetPSMat(m_stage->TransPosScrToProj(x, y));
	ps->Start();
	ps->Reset();

	return false;
}

bool EditOP::OnMouseDrag(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	ParticleSystem* ps = m_stage->m_ps;
	m_stage->SetPSMat(m_stage->TransPosScrToProj(x, y));

	return false;
}

}