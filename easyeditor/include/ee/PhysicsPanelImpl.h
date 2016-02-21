#ifndef _EASYEDITOR_PHYSICS_PANEL_IMPL_H_
#define _EASYEDITOR_PHYSICS_PANEL_IMPL_H_

#include "Box2DRender.h"

#include <Box2D/Box2D.h>

#include <wx/wx.h>

namespace ee
{

class PhysicsPanelImpl : public b2ContactListener, public wxEvtHandler
{
public:
	PhysicsPanelImpl();
	virtual ~PhysicsPanelImpl();

	virtual void update(wxCommandEvent& event);

	virtual void drawPhysics();

	b2World* createWorld(const Vector& gravity);

	void pause();

	b2World* getWorld() { return m_world; }

protected:
	b2World* m_world;

	int m_velocityIterations;
	int m_positionIterations;

private:
	Box2DRender m_debug_draw;

	wxTimer* m_timer;
	bool m_isPaused;

}; // PhysicsPanelImpl

}

#endif // _EASYEDITOR_PHYSICS_PANEL_IMPL_H_
