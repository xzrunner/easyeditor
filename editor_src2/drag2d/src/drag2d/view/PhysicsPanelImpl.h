#pragma once

#include "render/b2Render.h"

#include <Box2D/Box2D.h>
#include <wx/wx.h>

namespace d2d
{
	class Vector;

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
		b2GLRender m_debugDraw;

		wxTimer* m_timer;
		bool m_isPaused;

	}; // PhysicsPanelImpl
}

