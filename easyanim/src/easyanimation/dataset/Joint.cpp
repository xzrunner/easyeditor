#include "Joint.h"

#include <ee/Sprite.h>
#include <ee/EE_RVG.h>
#include <ee/Math2D.h>

#include <assert.h>

namespace eanim
{

static const int REGION = 5;

Joint::Joint(ee::Sprite* sprite)
	: m_sprite(sprite)
	, m_parent(NULL)
{
	m_relativeAngle = 0;
	CreateId();
}

Joint::Joint(ee::Sprite* sprite, const ee::Vector& pos)
	: m_sprite(sprite)
	, m_parent(NULL)
{
	SetPosition(pos);
	m_relativeAngle = sprite->GetAngle();
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
	ee::RVG::Circle(GetWorldPos(), REGION, true, ee::Colorf(0.2f, 0.8f, 0.2f, 0.5f));
	ee::RVG::Circle(GetWorldPos(), REGION, false, ee::Colorf(0.8f, 0.2f, 0.2f, 0.5f));
	if (m_parent)
	{
		ee::Vector s = GetWorldPos();
		ee::Vector e = m_sprite->GetPosition() * 2 - s;

		const float w = 0.1f;
		ee::Vector mid = s + (e-s)*w;
		ee::Vector left = mid + ee::Math2D::RotateVectorRightAngle(s - mid, false);
		ee::Vector right = mid + ee::Math2D::RotateVectorRightAngle(s - mid, true);

		ee::RVG::Line(s, left, ee::Colorf(0.8f, 0.2f, 0.2f, 0.5f));
		ee::RVG::Line(left, e, ee::Colorf(0.8f, 0.2f, 0.2f, 0.5f));
		ee::RVG::Line(e, right, ee::Colorf(0.8f, 0.2f, 0.2f, 0.5f));
		ee::RVG::Line(right, s, ee::Colorf(0.8f, 0.2f, 0.2f, 0.5f));
	}
}

bool Joint::Contain(const ee::Vector& pos) const
{
	return ee::Math2D::GetDistance(pos, GetWorldPos()) < REGION;
}

bool Joint::Intersect(const ee::Vector& pos) const
{
	return ee::Math2D::GetDistance(pos, GetWorldPos()) < REGION * 2;
}

void Joint::SetPosition(const ee::Vector& pos) 
{
	m_relative = GetRelativePos(pos);
}

ee::Vector Joint::GetWorldPos() const
{
	return m_sprite->GetPosition() + ee::Math2D::RotateVector(m_relative, m_sprite->GetAngle());
}

ee::Vector Joint::GetRelativePos(const ee::Vector& pos) const
{
	return ee::Math2D::RotateVector(pos - m_sprite->GetPosition(), -m_sprite->GetAngle());
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

}