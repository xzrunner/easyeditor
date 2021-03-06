#include "UniversalArrangeOP.h"
#include "DragPhysicsOP.h"
#include "PhysicsQuery.h"

namespace ee
{

UniversalArrangeOP::UniversalArrangeOP(wxWindow* wnd, EditPanelImpl* stage, MultiSpritesImpl* sprites_impl,
									   PropertySettingPanel* property, EditCMPT* callback/* = NULL*/)
	: ZoomViewOP(wnd, stage, true)
{
	m_no_physics = std::make_shared<ArrangeSpriteOP<SelectSpritesOP>>(wnd, stage, sprites_impl, property, callback);
	m_editop = m_no_physics;
}

bool UniversalArrangeOP::OnKeyDown(int keyCode)
{
	return m_editop->OnKeyDown(keyCode);	
}

bool UniversalArrangeOP::OnMouseLeftDown(int x, int y)
{
	m_editop = m_no_physics;

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	for (size_t i = 0, n = m_physics.size(); i < n; ++i)
	{
		if (PhysicsQuery::QueryOn(m_physics[i].world, pos))
		{
			m_editop = m_physics[i].editop;
			break;
		}
	}

	return m_editop->OnMouseLeftDown(x, y);
}

bool UniversalArrangeOP::OnMouseLeftUp(int x, int y)
{
	return m_editop->OnMouseLeftUp(x, y);
}

bool UniversalArrangeOP::OnMouseRightDown(int x, int y)
{
	return m_editop->OnMouseRightDown(x, y);
}

bool UniversalArrangeOP::OnMouseRightUp(int x, int y)
{
	return m_editop->OnMouseRightUp(x, y);
}

bool UniversalArrangeOP::OnMouseDrag(int x, int y)
{
	return m_editop->OnMouseDrag(x, y);
}

bool UniversalArrangeOP::OnDraw() const
{
	return m_editop->OnDraw();
}

bool UniversalArrangeOP::Clear()
{
	return m_editop->Clear();
}

void UniversalArrangeOP::addPhysicsEditOP(b2World* world, b2Body* ground)
{
	PhysicsOP physics;
	physics.world = world;
	physics.editop = std::make_shared<DragPhysicsOP>(m_wnd, m_stage, world, ground);
	m_physics.push_back(physics);
}

}
