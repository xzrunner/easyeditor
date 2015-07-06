#pragma once

#include "AbstractBV.h"

#include "common/Rect.h"

namespace d2d
{
	class OBB : public AbstractBV
	{
	public:
		OBB();
		OBB(const OBB& obb);

		//
		// IObject interface
		//
		virtual OBB* Clone() const;

		virtual void initFromRect(const Rect& rect);

		virtual bool isValid() const;
		virtual void makeInfinite();

		virtual void SetTransform(const Vector& position, const Vector& offset, float angle);
		virtual void SetMirror(bool xmirror, bool ymirror);

		virtual void combine(const Vector& pos);
		virtual void combine(const Rect& rect);

		virtual bool isContain(const Vector& pos) const;
		virtual bool isContain(const Rect& rect) const;
		virtual bool isIntersect(const Rect& rect) const;

		virtual float area() const;
		virtual float width() const;
		virtual float height() const;
		virtual Vector center() const;

		virtual void getBoundPos(std::vector<Vector>& bound) const;

	private:
		void transToAABB();

	private:
		Rect m_rect;
		Vector m_position;
		float m_angle;

	}; // OBB

	inline void OBB::initFromRect(const Rect& rect)
	{
 		m_rect = rect;
 		m_position.set(0, 0);
 		m_angle = 0;
	}

	inline bool OBB::isValid() const
	{
		return m_rect.isValid();
	}

	inline void OBB::makeInfinite()
	{
		m_rect.makeInfinite();
	}

	inline void OBB::combine(const Vector& pos)
	{
		if (m_rect.isValid())
			transToAABB();
		m_rect.combine(pos - m_position);
	}

	inline void OBB::combine(const Rect& rect)
	{
		if (m_rect.isValid())
			transToAABB();
		Rect r(rect);
		r.translate(-m_position);
		m_rect.combine(r);
	}
}

