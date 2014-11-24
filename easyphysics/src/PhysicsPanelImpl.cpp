#include "PhysicsPanelImpl.h"
#include "BodyFactory.h"
#include "const.h"

namespace ephysics
{

PhysicsPanelImpl::PhysicsPanelImpl()
	: m_debug_draw(BOX2D_SCALE_FACTOR)
	, m_velocityIterations(8)
	, m_positionIterations(3)
{
	m_world = createWorld(d2d::Vector(0.0f, -10.0f));

	BodyFactory::setWorld(m_world);

	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
	m_debug_draw.SetFlags(flags);

	m_timer = new wxTimer(this, 1);
	m_timer->Start(1.0f / 60);
	m_isPaused = false;

	Connect(wxEVT_TIMER, wxCommandEventHandler(PhysicsPanelImpl::update));
}

PhysicsPanelImpl::~PhysicsPanelImpl()
{
	pause();
}

void PhysicsPanelImpl::update(wxCommandEvent& event)
{
	const float timeStep = 1.0f / 60;
	m_world->Step(timeStep, m_velocityIterations, m_positionIterations);
}

void PhysicsPanelImpl::drawPhysics()
{
	m_world->DrawDebugData();
}

b2World* PhysicsPanelImpl::createWorld(const d2d::Vector& gravity)
{
	b2World* world;

	b2Vec2 b2gravity;
	b2gravity.Set(gravity.x, gravity.y);
	world = new b2World(b2gravity);

	world->SetContactListener(this);
	world->SetDebugDraw(&m_debug_draw);

	return world;
}

void PhysicsPanelImpl::pause()
{
	m_isPaused = !m_isPaused;
	if (m_isPaused)
		m_timer->Stop();
	else
		m_timer->Start(1.0f / 60);
}

}
