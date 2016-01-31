
#include "Joint.h"
#include "Body.h"

using namespace libmodeling;

const float Joint::JOINT_RADIUS_OUT = 1.0f;
const float Joint::JOINT_RADIUS_IN = 0.2f;
const float Joint::JOINT_RADIUS_SELECT = JOINT_RADIUS_OUT * 3;

Joint::Joint(Body* b0, Body* b1, Type type)
	: bodyA(b0)
	, bodyB(b1)
	, type(type)
	, collideConnected(false)
{
	static int count = 0;
	m_name = wxT("joint") + wxString::FromDouble(count++);
}

void Joint::drawBodyFlag() const
{
	drawBodyFlag(bodyA->sprite->GetPosition());
	drawBodyFlag(bodyB->sprite->GetPosition());
}

ee::Vector Joint::transWorldToLocal(const ee::Vector& world, 
									const ee::Sprite* sprite)
{
	return ee::Math2D::RotateVector(world - sprite->GetPosition(), -sprite->GetAngle());
}

ee::Vector Joint::transLocalToWorld(const ee::Vector& local, 
									const ee::Sprite* sprite)
{
	return ee::Math2D::RotateVector(local, sprite->GetAngle()) + sprite->GetPosition();
}

void Joint::drawBodyFlag(const ee::Vector& pos) const
{
	const float edge = 2.5f;

	std::vector<ee::Vector> vertices(4, pos);
	vertices[0].x -= edge;
	vertices[1].x += edge;
	vertices[2].y -= edge;
	vertices[3].y += edge;
	ee::PrimitiveDraw::DrawLines(vertices, ee::Colorf(0.4f, 0.8f, 0.8f), 1);
}
