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
		virtual OBB* clone();

		//
		// ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

		virtual void initFromRect(const Rect& rect);

		virtual bool isValid() const;
		virtual void makeInfinite();

		virtual void translate(const Vector& offset);
		virtual void rotate(float delta);

		virtual void combine(const Vector& pos);
		virtual void combine(const Rect& rect);

		virtual bool isContain(const Vector& pos) const;
		virtual bool isIntersect(const Rect& rect) const;

		virtual float area() const;
		virtual float width() const;
		virtual float height() const;
		virtual Vector center() const;

		virtual void getBoundPos(std::vector<Vector>& bound) const;

		const Rect& getRect() const;
		float getAngle() const;

	private:
		Rect transToRect() const;

	private:
		Rect m_rect;
		float m_angle;

	}; // OBB

	inline void OBB::initFromRect(const Rect& rect)
	{
		m_rect = rect;
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
		{
			m_rect = transToRect();
			m_angle = 0;
		}
		m_rect.combine(pos);
	}

	inline void OBB::combine(const Rect& rect)
	{
		if (m_rect.isValid())
		{
			m_rect = transToRect();
			m_angle = 0;
		}
		m_rect.combine(rect);
	}

	inline const Rect& OBB::getRect() const
	{
		return m_rect;
	}

	inline float OBB::getAngle() const
	{
		return m_angle;
	}
}

