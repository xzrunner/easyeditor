#ifndef _EPHYSICS_INTERFACE_BODY_H_
#define _EPHYSICS_INTERFACE_BODY_H_

#include <wx/string.h>
#include <Box2D/Box2D.h>
#include <drag2d.h>

namespace ephysics
{

class IBody : public d2d::Cloneable, public d2d::Object
{
public:
	virtual ~IBody();

	//
	// Cloneable interface
	//
	virtual IBody* Clone() const;

	virtual void getRect(d2d::Rect& rect, const d2d::Vector& position, float angle) const = 0;

	b2Body* getBody() const;
	void setBody(b2Body* body);

	const std::string& getFilename() const;

	bool isAlive() const;
	void setAlive(bool isAlive);

	void setDensity(float density);
	void setFriction(float friction);
	void setCollisionFilter(const b2Filter& filter);

	void setTransform(const d2d::Vector& pos, float angle);

protected:
	IBody() {}
	IBody(b2World* world, float scale = 1.0f);

protected:
	std::string m_filename;

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

inline const std::string& IBody::getFilename() const
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

#endif // _EPHYSICS_INTERFACE_BODY_H_