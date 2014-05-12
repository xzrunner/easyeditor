#pragma once

#include <wx/wx.h>

#include "interfaces.h"

class b2World;
class b2Body;
struct b2Filter;

namespace d2d
{
	class Rect;
	class Vector;

	class IBody : public ICloneable
	{
	public:
		virtual ~IBody();

		//
		// ICloneable interface
		//
		virtual IBody* clone() const;

		virtual void getRect(Rect& rect, const Vector& position, float angle) const = 0;

		b2Body* getBody() const;
		void setBody(b2Body* body);

		const wxString& getFilename() const;

		bool isAlive() const;
		void setAlive(bool isAlive);

		void setDensity(float density);
		void setFriction(float friction);
		void setCollisionFilter(const b2Filter& filter);

		void setTransform(const Vector& pos, float angle);

	protected:
		IBody() {}
		IBody(b2World* world, float scale = 1.0f);

	protected:
		wxString m_filename;

		bool m_alive;

		b2World* m_world;
		b2Body* m_body;

	private:
		float m_scale;

	}; // IBody

	inline b2Body* IBody::getBody() const
	{
		return m_body;
	}

	inline void IBody::setBody(b2Body* body)
	{
		m_body = body;
	}

	inline const wxString& IBody::getFilename() const
	{
		return m_filename;
	}

	inline bool IBody::isAlive() const
	{
		return m_alive;
	}

	inline void IBody::setAlive(bool isAlive)
	{
		m_alive = isAlive;
	}
}

