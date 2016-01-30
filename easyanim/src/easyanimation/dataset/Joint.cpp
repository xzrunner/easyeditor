#include "Joint.h"

namespace eanim
{

static const int REGION = 5;

Joint::Joint(d2d::Sprite* sprite)
	: m_sprite(sprite)
	, m_parent(NULL)
{
	m_relativeAngle = 0;
	CreateId();
}

Joint::Joint(d2d::Sprite* sprite, const d2d::Vector& pos)
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
	d2d::PrimitiveDraw::DrawCircle(GetWorldPos(), REGION, true, 2, d2d::Colorf(0.2f, 0.8f, 0.2f, 0.5f));
	d2d::PrimitiveDraw::DrawCircle(GetWorldPos(), REGION, false, 2, d2d::Colorf(0.8f, 0.2f, 0.2f, 0.5f));
	if (m_parent)
	{
		d2d::Vector s = GetWorldPos();
		d2d::Vector e = m_sprite->GetPosition() * 2 - s;

		const float w = 0.1f;
		d2d::Vector mid = s + (e-s)*w;
		d2d::Vector left = mid + d2d::Math2D::RotateVectorRightAngle(s - mid, false);
		d2d::Vector right = mid + d2d::Math2D::RotateVectorRightAngle(s - mid, true);

		d2d::PrimitiveDraw::DrawLine(s, left, d2d::Colorf(0.8f, 0.2f, 0.2f, 0.5f));
		d2d::PrimitiveDraw::DrawLine(left, e, d2d::Colorf(0.8f, 0.2f, 0.2f, 0.5f));
		d2d::PrimitiveDraw::DrawLine(e, right, d2d::Colorf(0.8f, 0.2f, 0.2f, 0.5f));
		d2d::PrimitiveDraw::DrawLine(right, s, d2d::Colorf(0.8f, 0.2f, 0.2f, 0.5f));
	}
}

bool Joint::Contain(const d2d::Vector& pos) const
{
	return d2d::Math2D::GetDistance(pos, GetWorldPos()) < REGION;
}

bool Joint::Intersect(const d2d::Vector& pos) const
{
	return d2d::Math2D::GetDistance(pos, GetWorldPos()) < REGION * 2;
}

void Joint::SetPosition(const d2d::Vector& pos) 
{
	m_relative = GetRelativePos(pos);
}

d2d::Vector Joint::GetWorldPos() const
{
	return m_sprite->GetPosition() + d2d::Math2D::RotateVector(m_relative, m_sprite->GetAngle());
}

d2d::Vector Joint::GetRelativePos(const d2d::Vector& pos) const
{
	return d2d::Math2D::RotateVector(pos - m_sprite->GetPosition(), -m_sprite->GetAngle());
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