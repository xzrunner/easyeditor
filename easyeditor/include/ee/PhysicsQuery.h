#ifndef _EASYEDITOR_PHYSICS_QUERY_H_
#define _EASYEDITOR_PHYSICS_QUERY_H_

#include <Box2D/Box2D.h>

namespace ee
{

class Vector;

class PhysicsQuery
{
public:
	static b2Body* QueryOn(b2World* world, const Vector& pos);
	static b2Body* QueryNear(b2World* world, const Vector& pos);

private:
	class QueryCallback : public b2QueryCallback
	{
	public:
		QueryCallback(const b2Vec2& point);

		bool ReportFixture(b2Fixture* fixture);

	public:
		b2Vec2 m_point;
		b2Fixture* m_fixture;

	}; // QueryCallback

}; // PhysicsQuery

}

#endif // _EASYEDITOR_PHYSICS_QUERY_H_