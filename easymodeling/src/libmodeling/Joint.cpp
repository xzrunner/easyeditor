#include "Joint.h"
#include "Body.h"

#include <ee/StringHelper.h>
#include <ee/Sprite.h>
#include <ee/Math2D.h>
#include <ee/EE_RVG.h>

namespace emodeling
{

const float Joint::JOINT_RADIUS_OUT = 1.0f;
const float Joint::JOINT_RADIUS_IN = 0.2f;
const float Joint::JOINT_RADIUS_SELECT = JOINT_RADIUS_OUT * 3;

Joint::Joint(Body* b0, Body* b1, Type type)
	: m_body_a(b0)
	, m_body_b(b1)
	, m_type(type)
	, m_collide_connected(false)
{
	static int count = 0;
	m_name = std::string("joint") + ee::StringHelper::ToString(count++);
}

void Joint::DrawBodyFlag() const
{
	DrawBodyFlag(m_body_a->m_sprite->GetPosition());
	DrawBodyFlag(m_body_b->m_sprite->GetPosition());
}

ee::Vector Joint::TransWorldToLocal(const ee::Vector& world, 
									const ee::Sprite* sprite)
{
	return ee::Math2D::RotateVector(world - sprite->GetPosition(), -sprite->GetAngle());
}

ee::Vector Joint::TransLocalToWorld(const ee::Vector& local, 
									const ee::Sprite* sprite)
{
	return ee::Math2D::RotateVector(local, sprite->GetAngle()) + sprite->GetPosition();
}

void Joint::DrawBodyFlag(const ee::Vector& pos) const
{
	const float edge = 2.5f;

	std::vector<ee::Vector> vertices(4, pos);
	vertices[0].x -= edge;
	vertices[1].x += edge;
	vertices[2].y -= edge;
	vertices[3].y += edge;
	ee::RVG::Color(ee::Colorf(0.4f, 0.8f, 0.8f));
	ee::RVG::LineWidth(1);
	ee::RVG::Lines(vertices);
	ee::RVG::LineWidth(2);
}

}