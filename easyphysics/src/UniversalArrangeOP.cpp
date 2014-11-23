#include "UniversalArrangeOP.h"
#include "DragPhysicsOP.h"
#include "PhysicsQuery.h"

namespace ephysics
{

UniversalArrangeOP::UniversalArrangeOP(d2d::EditPanel* editPanel, d2d::MultiSpritesImpl* spritesImpl,
									   d2d::PropertySettingPanel* propertyPanel, d2d::AbstractEditCMPT* callback/* = NULL*/)
	: d2d::ZoomViewOP(editPanel, true)
{
	m_noPhysics = new d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>(editPanel, spritesImpl, propertyPanel, callback);
	m_editOP = m_noPhysics;
}

UniversalArrangeOP::~UniversalArrangeOP()
{
	m_noPhysics->Release();
	m_noPhysics = NULL;

	for (size_t i = 0, n = m_physics.size(); i < n; ++i)
		m_physics[i].editOP->Release();
	m_physics.clear();

	m_editOP = NULL;
}

bool UniversalArrangeOP::onKeyDown(int keyCode)
{
	return m_editOP->onKeyDown(keyCode);	
}

bool UniversalArrangeOP::onMouseLeftDown(int x, int y)
{
	m_editOP = m_noPhysics;

	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	for (size_t i = 0, n = m_physics.size(); i < n; ++i)
	{
		if (PhysicsQuery::queryOn(m_physics[i].world, pos))
		{
			m_editOP = m_physics[i].editOP;
			break;
		}
	}

	return m_editOP->onMouseLeftDown(x, y);
}

bool UniversalArrangeOP::onMouseLeftUp(int x, int y)
{
	return m_editOP->onMouseLeftUp(x, y);
}

bool UniversalArrangeOP::onMouseRightDown(int x, int y)
{
	return m_editOP->onMouseRightDown(x, y);
}

bool UniversalArrangeOP::onMouseRightUp(int x, int y)
{
	return m_editOP->onMouseRightUp(x, y);
}

bool UniversalArrangeOP::onMouseDrag(int x, int y)
{
	return m_editOP->onMouseDrag(x, y);
}

bool UniversalArrangeOP::onDraw() const
{
	return m_editOP->onDraw();
}

bool UniversalArrangeOP::clear()
{
	return m_editOP->clear();
}

void UniversalArrangeOP::addPhysicsEditOP(b2World* world, b2Body* ground)
{
	PhysicsOP physics;
	physics.world = world;
	physics.editOP = new DragPhysicsOP(m_editPanel, world, ground);
	m_physics.push_back(physics);
}

}
