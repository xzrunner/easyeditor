#include "EditOP.h"
#include "StagePanel.h"
#include "MotionTrail.h"

namespace etrail
{

EditOP::EditOP(StagePanel* stage)
	: ee::ZoomViewOP(stage, stage->GetStageImpl(), true)
	, m_stage(stage)
{
}

bool EditOP::OnMouseLeftDown(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	m_stage->SetTrailMat(m_stage->TransPosScrToProj(x, y));
	m_stage->m_trail->Start();

	return false;
}

bool EditOP::OnMouseDrag(int x, int y)
{
	if (ee::ZoomViewOP::OnMouseLeftDown(x, y)) return true;

	m_stage->SetTrailMat(m_stage->TransPosScrToProj(x, y));

	return false;
}


}