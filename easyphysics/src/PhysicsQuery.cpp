#include "PhysicsQuery.h"
#include "const.h"

namespace ephysics
{

b2Body* PhysicsQuery::queryOn(b2World* world, const d2d::Vector& pos)
{
	const b2Vec2 b2Pos(pos.x / BOX2D_SCALE_FACTOR, pos.y / BOX2D_SCALE_FACTOR);

	// Make a small box.
	b2AABB aabb;
	b2Vec2 d;
	d.Set(0.001f, 0.001f);
	aabb.lowerBound = b2Pos - d;
	aabb.upperBound = b2Pos + d;

	// Query the world for overlapping shapes.
	QueryCallback callback(b2Pos);
	world->QueryAABB(&callback, aabb);

	if (callback.m_fixture)
		return callback.m_fixture->GetBody();
	else
		return NULL;
}

b2Body* PhysicsQuery::queryNear(b2World* world, const d2d::Vector& pos)
{
	b2Body* onBody = queryOn(world, pos);
	if (onBody)
	{
		return onBody;
	}
	else
	{
		b2Body* nearBody = NULL;
		float nearDis = FLT_MAX;
		for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
		{
			b2Vec2 bPos = b->GetPosition();
			const float dis = d2d::Math::getDistance(pos, d2d::Vector(bPos.x, bPos.y));
			if (dis < nearDis)
			{
				nearDis = dis;
				nearBody = b;
			}
		}
		return nearBody;
	}
}

//////////////////////////////////////////////////////////////////////////
// class PhysicsQuery::QueryCallback
//////////////////////////////////////////////////////////////////////////

PhysicsQuery::QueryCallback::QueryCallback(const b2Vec2& point)
{
	m_point = point;
	m_fixture = NULL;
}

bool PhysicsQuery::QueryCallback::ReportFixture(b2Fixture* fixture)
{
	b2Body* body = fixture->GetBody();
	if (body->GetType() == b2_dynamicBody)
	{
		bool inside = fixture->TestPoint(m_point);
		if (inside)
		{
			m_fixture = fixture;

			// We are done, terminate the query.
			return false;
		}
	}

	// Continue the query.
	return true;
}

} // d2d