#include "PreviewPanel.h"
#include "PreviewCanvas.h"
#include "StagePanel.h"
#include "ResolveToB2.h"
#include "Context.h"

#include <ee/DragPhysicsOP.h>

#include <easymodeling.h>

namespace emodeling
{

PreviewPanel::PreviewPanel(wxWindow* parent,
						   wxTopLevelWindow* frame)
	: ee::EditPanel(parent, frame)
{
	createGround();

	SetCanvas(new PreviewCanvas(this));
	SetEditOP(new ee::DragPhysicsOP(this, GetStageImpl(), m_world, m_ground));

	std::map<Body*, b2Body*> mapBody;
	Context::Instance()->stage->traverseBodies(LoadBodyVisitor(m_world, mapBody));
	std::map<Joint*, b2Joint*> mapJoint;
	Context::Instance()->stage->traverseJoints(LoadJointVisitor(m_world, mapBody, mapJoint));
	Context::Instance()->stage->traverseJoints(LoadGearJointVisitor(m_world, mapBody, mapJoint));

	const World* world = Context::Instance()->world;
	m_world->SetGravity(b2Vec2(world->gravity.x, world->gravity.y));
	m_world->SetAllowSleeping(world->allowSleep);
	m_world->SetWarmStarting(world->warmStarting);
	m_world->SetContinuousPhysics(world->continuousPhysics);
	m_world->SetSubStepping(world->subStepping);
	m_velocityIterations = world->velocityIterations;
	m_positionIterations = world->positionIterations;
}

void PreviewPanel::createGround()
{
	const float halfEdge = 30.0f;

	b2Vec2 vertices[4];
	vertices[0].Set(-halfEdge, -halfEdge);
	vertices[1].Set( halfEdge, -halfEdge);
	vertices[2].Set( halfEdge,  halfEdge);
	vertices[3].Set(-halfEdge,  halfEdge);

	b2BodyDef bd;
	bd.type = b2_staticBody;
	m_ground = m_world->CreateBody(&bd);

	b2ChainShape shape;
	shape.CreateLoop(&vertices[0], 4);

	b2FixtureDef fd;
	fd.shape = &shape;

	m_ground->CreateFixture(&fd);
}

//////////////////////////////////////////////////////////////////////////
// class PreviewPanel::LoadBodyVisitor
//////////////////////////////////////////////////////////////////////////

PreviewPanel::LoadBodyVisitor::
LoadBodyVisitor(b2World* world, std::map<Body*, b2Body*>& mapBody) 
	: m_world(world)
	, m_mapBody(mapBody)
{}

void PreviewPanel::LoadBodyVisitor::
Visit(ee::Object* object, bool& next)
{
	Body* data = static_cast<Body*>(object);

	b2Body* body = ResolveToB2::CreateBody(*data, m_world, m_mapBody);

	next = true;
}

//////////////////////////////////////////////////////////////////////////
// class PreviewPanel::LoadJointVisitor
//////////////////////////////////////////////////////////////////////////

PreviewPanel::LoadJointVisitor::
LoadJointVisitor(b2World* world, const std::map<Body*, b2Body*>& mapBody,
				 std::map<Joint*, b2Joint*>& mapJoint) 
	: m_world(world) 
	, m_mapBody(mapBody)
	, m_mapJoint(mapJoint)
{}

void PreviewPanel::LoadJointVisitor::
Visit(ee::Object* object, bool& next)
{
	Joint* data = static_cast<Joint*>(object);

	b2Joint* joint = ResolveToB2::CreateJoint(*data, m_world, m_mapBody);
	if (joint)
		m_mapJoint.insert(std::make_pair(data, joint));

	next = true;
}

//////////////////////////////////////////////////////////////////////////
// class PreviewPanel::LoadGearJointVisitor
//////////////////////////////////////////////////////////////////////////

PreviewPanel::LoadGearJointVisitor::
LoadGearJointVisitor(b2World* world, const std::map<Body*, b2Body*>& mapBody,
					 const std::map<Joint*, b2Joint*>& mapJoint) 
	: m_world(world) 
	, m_mapBody(mapBody)
	, m_mapJoint(mapJoint)
{}

void PreviewPanel::LoadGearJointVisitor::
Visit(ee::Object* object, bool& next)
{
	Joint* data = static_cast<Joint*>(object);

	ResolveToB2::CreateJoint(*data, m_world, m_mapBody, m_mapJoint);

	next = true;
}

}