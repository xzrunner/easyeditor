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
	ps->SetPosition(m_stage->TransPosScrToProj(x, y));
	ps->Start();
	ps->Reset();
	ps->SetLoop(true);

	return false;
}

bool EditOP::OnMouseLeftUp(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftUp(x, y)) return true;

	ParticleSystem* ps = m_stage->m_ps;
	ps->SetLoop(false);

	return false;
}

bool EditOP::OnMouseRightDown(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseRightDown(x, y)) return true;

	ParticleSystem* ps = m_stage->m_ps;
	ps->SetPosition(m_stage->TransPosScrToProj(x, y));
	ps->Start();
	ps->Reset();
	ps->SetLoop(false);

	return false;
}

bool EditOP::OnMouseDrag(int x, int y)
{
	if (d2d::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	ParticleSystem* ps = m_stage->m_ps;
	ps->SetPosition(m_stage->TransPosScrToProj(x, y));

	return false;
}

}