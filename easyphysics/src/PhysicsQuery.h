#ifndef _EPHYSICS_PHYSICS_QUERY_H_
#define _EPHYSICS_PHYSICS_QUERY_H_


#include <Box2D/Box2D.h>

namespace ephysics
{

class PhysicsQuery
{
public:
	static b2Body* queryOn(b2World* world, const ee::Vector& pos);
	static b2Body* queryNear(b2World* world, const ee::Vector& pos);

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

#endif // _EPHYSICS_PHYSICS_QUERY_H_