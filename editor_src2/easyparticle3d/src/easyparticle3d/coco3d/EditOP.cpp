#include "EditOP.h"
#include "StagePanel.h"

using namespace eparticle;

namespace eparticle
{
namespace coco3d
{
	EditOP::EditOP(StagePanel* stage)
		: d2d::AbstractEditOP(stage)
		, m_stage(stage)
	{
	}

	bool EditOP::onKeyDown(int keyCode)
	{
		switch (keyCode)
		{
		case 'w': case 'W':
			m_stage->xRot -= 5;
			break;
		case 's': case 'S':
			m_stage->xRot += 5;
			break;
		case 'a': case 'A':
			m_stage->yRot -= 5;
			break;
		case 'd': case 'D':
			m_stage->yRot += 5;
			break;
		}

		m_editPanel->Refresh();

		return false;
	}

	bool EditOP::onMouseLeftDown(int x, int y)
	{
// 		if (m_stage->m_particle)
// 		{
// 			d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
// 			m_stage->m_particle->setPosition(pos.x, pos.y);
// 			m_stage->m_particle->start();
// 		}

		return false;
	}

	bool EditOP::onMouseDrag(int x, int y)
	{
// 		if (m_stage->m_particle)
// 		{
// 			d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
// 			m_stage->m_particle->setPosition(pos.x, pos.y);
// 			m_stage->m_particle->start();
// 		}

		return false;
	}

	bool EditOP::onMouseMove(int x, int y)
	{
		m_editPanel->SetFocus();
		return false;
	}

	bool EditOP::onMouseWheelRotation(int x, int y, int direction)
	{
		if (direction > 0) 
			m_stage->xRot -= 5;
		else
			m_stage->xRot += 5;
		return false;
	}
}
}