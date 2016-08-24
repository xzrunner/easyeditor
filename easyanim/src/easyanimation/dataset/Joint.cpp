#include "Joint.h"

#include <ee/Sprite.h>
#include <ee/Math2D.h>

#include <sprite2/Color.h>
#include <sprite2/S2_RVG.h>
#include <SM_Calc.h>

#include <assert.h>

namespace eanim
{

static const int REGION = 5;

Joint::Joint(ee::Sprite* spr)
	: m_spr(spr)
	, m_parent(NULL)
{
	m_relative_angle = 0;
	CreateId();
}

Joint::Joint(ee::Sprite* spr, const sm::vec2& pos)
	: m_spr(spr)
	, m_parent(NULL)
{
	SetPosition(pos);
	m_relative_angle = spr->GetAngle();
	CreateId();
}

Joint::~Joint()
{
	if (m_parent)
		m_parent->m_children.erase(this);

	std::set<Joint*>::iterator itr = m_children.begin();
	for ( ; itr != m_children.end(); ++itr)
	{
		Joint* child = *itr;
		assert(child->m_parent == this);
		child->m_parent = NULL;
	}
}

void Joint::Draw() const
{
	s2::RVG::SetColor(s2::Color(51, 204, 51, 128));
	s2::RVG::Circle(GetWorldPos(), REGION, true);
	s2::RVG::SetColor(s2::Color(204, 51, 51, 128));
	s2::RVG::Circle(GetWorldPos(), REGION, false);
	if (m_parent)
	{
		sm::vec2 s = GetWorldPos();
		sm::vec2 e = m_spr->GetPosition() * 2 - s;

		const float w = 0.1f;
		sm::vec2 mid = s + (e-s)*w;
		sm::vec2 left = mid + sm::rotate_vector_right_angle(s - mid, false);
		sm::vec2 right = mid + sm::rotate_vector_right_angle(s - mid, true);

		s2::RVG::SetColor(s2::Color(204, 51, 51, 128));
		s2::RVG::Line(s, left);
		s2::RVG::Line(left, e);
		s2::RVG::Line(e, right);
		s2::RVG::Line(right, s);
	}
}

bool Joint::Contain(const sm::vec2& pos) const
{
	return sm::dis_pos_to_pos(pos, GetWorldPos()) < REGION;
}

bool Joint::Intersect(const sm::vec2& pos) const
{
	return sm::dis_pos_to_pos(pos, GetWorldPos()) < REGION * 2;
}

void Joint::SetPosition(const sm::vec2& pos) 
{
	m_relative_pos = GetRelativePos(pos);
}

sm::vec2 Joint::GetWorldPos() const
{
	return m_spr->GetPosition() + sm::rotate_vector(GetRelativePos(), m_spr->GetAngle());
}

sm::vec2 Joint::GetRelativePos(const sm::vec2& pos) const
{
	return sm::rotate_vector(pos - m_spr->GetPosition(), -m_spr->GetAngle());
}

bool Joint::Connect(Joint* joint)
{
	std::set<Joint*>::iterator itr = joint->m_children.find(this);
	if (itr == joint->m_children.end())
	{
		joint->m_parent = this;
		m_children.insert(joint);
		return true;
	}
	else
	{
		return false;
	}
}

void Joint::Deconnect()
{
	if (m_parent)
	{
		m_parent->m_children.erase(this);
		m_parent = NULL;
	}
}

void Joint::CreateId()
{
	static int id = 0;
	m_id = ++id;
}

sm::vec2 Joint::GetRelativePos() const
{
	sm::vec2 pos = m_relative_pos;
	pos.x *= m_spr->GetScale().x;
	pos.y *= m_spr->GetScale().y;
	return pos;
}

}