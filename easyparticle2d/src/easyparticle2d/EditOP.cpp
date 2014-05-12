#include "EditOP.h"
#include "StagePanel.h"

namespace eparticle2d
{

EditOP::EditOP(StagePanel* stage)
	: d2d::ZoomViewOP(stage, true)
	, m_stage(stage)
{
}

bool EditOP::onMouseLeftDown(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseLeftDown(x, y)) return true;

 	if (m_stage->m_particle)
 	{
 		d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
 		m_stage->m_particle->setPosition(pos.x, pos.y);
 		m_stage->m_particle->start();
 	}

	return false;
}

bool EditOP::onMouseDrag(int x, int y)
{
	if (d2d::ZoomViewOP::onMouseLeftDown(x, y)) return true;

 	if (m_stage->m_particle)
 	{
 		d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
 		m_stage->m_particle->setPosition(pos.x, pos.y);
 		m_stage->m_particle->start();
 	}

	return false;
}

}