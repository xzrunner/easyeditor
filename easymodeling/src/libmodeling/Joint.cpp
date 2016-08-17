#include "Joint.h"
#include "Body.h"

#include <ee/StringHelper.h>
#include <ee/Sprite.h>
#include <ee/Math2D.h>

#include <sprite2/S2_RVG.h>

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
	DrawBodyFlag(m_body_a->m_spr->GetPosition());
	DrawBodyFlag(m_body_b->m_spr->GetPosition());
}

sm::vec2 Joint::TransWorldToLocal(const sm::vec2& world, 
									const ee::Sprite* spr)
{
	return sm::rotate_vector(world - spr->GetPosition(), -spr->GetAngle());
}

sm::vec2 Joint::TransLocalToWorld(const sm::vec2& local, 
									const ee::Sprite* spr)
{
	return sm::rotate_vector(local, spr->GetAngle()) + spr->GetPosition();
}

void Joint::DrawBodyFlag(const sm::vec2& pos) const
{
	const float edge = 2.5f;

	std::vector<sm::vec2> vertices(4, pos);
	vertices[0].x -= edge;
	vertices[1].x += edge;
	vertices[2].y -= edge;
	vertices[3].y += edge;
	s2::RVG::SetColor(s2::Color(0.4f, 0.8f, 0.8f));
	s2::RVG::LineWidth(1);
	s2::RVG::Lines(vertices);
	s2::RVG::LineWidth(2);
}

}