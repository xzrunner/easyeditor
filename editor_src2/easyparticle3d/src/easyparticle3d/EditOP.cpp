#include "EditOP.h"
#include "StagePanel.h"
#include "Context.h"

namespace eparticle3d
{

EditOP::EditOP(StagePanel* stage)
	: d2d::ZoomViewOP(stage, true)
	, m_stage(stage)
{
}

bool EditOP::onMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseLeftDown(x, y)) return true;

	ParticleSystem* ps = Context::Instance()->stage->m_particle;
	ps->origin = m_editPanel->transPosScreenToProject(x, y);
	ps->start();
	ps->reset();

	return false;
}

}