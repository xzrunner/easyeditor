#include "Joint.h"

namespace eanim
{
	const int Joint::REGION = 5;

	Joint::Joint(d2d::ISprite* sprite)
		: m_sprite(sprite)
		, m_parent(NULL)
	{
		m_relativeAngle = 0;
	}

	Joint::Joint(d2d::ISprite* sprite, const d2d::Vector& pos)
		: m_sprite(sprite)
		, m_parent(NULL)
	{
		setPosition(pos);
		m_relativeAngle = sprite->getAngle();
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

	void Joint::draw() const
	{
		d2d::PrimitiveDraw::drawCircle(getWorldPos(), Joint::REGION, true, 2, d2d::Colorf(0.2f, 0.8f, 0.2f, 0.5f));
		d2d::PrimitiveDraw::drawCircle(getWorldPos(), Joint::REGION, false, 2, d2d::Colorf(0.8f, 0.2f, 0.2f, 0.5f));
		if (m_parent)
		{
			d2d::Vector s = getWorldPos();
			d2d::Vector e = m_sprite->getPosition() * 2 - s;

			const float w = 0.1f;
			d2d::Vector mid = s + (e-s)*w;
			d2d::Vector left = mid + d2d::Math::rotateVectorRightAngle(s - mid, false);
			d2d::Vector right = mid + d2d::Math::rotateVectorRightAngle(s - mid, true);

 			d2d::PrimitiveDraw::drawLine(s, left, d2d::Colorf(0.8f, 0.2f, 0.2f, 0.5f));
 			d2d::PrimitiveDraw::drawLine(left, e, d2d::Colorf(0.8f, 0.2f, 0.2f, 0.5f));
 			d2d::PrimitiveDraw::drawLine(e, right, d2d::Colorf(0.8f, 0.2f, 0.2f, 0.5f));
 			d2d::PrimitiveDraw::drawLine(right, s, d2d::Colorf(0.8f, 0.2f, 0.2f, 0.5f));
		}
	}

	bool Joint::contain(const d2d::Vector& pos) const
	{
		return d2d::Math::getDistance(pos, getWorldPos()) < REGION;
	}

	bool Joint::intersect(const d2d::Vector& pos) const
	{
		return d2d::Math::getDistance(pos, getWorldPos()) < REGION * 2;
	}

	void Joint::setPosition(const d2d::Vector& pos) 
	{
		m_relative = getRelativePos(pos);
	}

	d2d::Vector Joint::getWorldPos() const
	{
		return m_sprite->getPosition() + d2d::Math::rotateVector(m_relative, m_sprite->getAngle());
	}
	
	d2d::Vector Joint::getRelativePos(const d2d::Vector& pos) const
	{
		return d2d::Math::rotateVector(pos - m_sprite->getPosition(), -m_sprite->getAngle());
	}

	void Joint::translate(const d2d::Vector& offset)
	{
		m_sprite->translate(offset);
	}

	void Joint::connect(Joint* joint)
	{
		std::set<Joint*>::iterator itr = joint->m_children.find(this);
		if (itr == joint->m_children.end())
		{
			joint->m_parent = this;
			m_children.insert(joint);
		}
	}

	void Joint::deconnect()
	{
		if (m_parent)
		{
			m_parent->m_children.erase(this);
			m_parent = NULL;
		}
	}
}