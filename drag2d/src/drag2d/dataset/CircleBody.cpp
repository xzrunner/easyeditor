#include "CircleBody.h"
#include "BodyData.h"

#include <Box2D/Box2D.h>

#include "common/Vector.h"
#include "common/Math.h"

namespace d2d
{

CircleBody::CircleBody(b2World* world, const BodyData& bodyData, float scale/* = 1.0f*/)
	: IBody(world, scale)
{
	m_filename = bodyData.getFilename();

	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	m_body = world->CreateBody(&bd);

	b2FixtureDef fd;
	fd.density = 1;
	fd.restitution = 0.6f;

	b2CircleShape shape;
	shape.m_radius = bodyData.getAllFixturesData()[0]->vertices[0].x * 0.5f * scale / BOX2D_SCALE_FACTOR;
	fd.shape = &shape;

	m_body->CreateFixture(&fd);
}

CircleBody::CircleBody(b2World* world, float radius)
	: IBody(world, 1.0f)
{
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	m_body = world->CreateBody(&bd);

	b2FixtureDef fd;
	fd.density = 1;
	fd.restitution = 0.6f;

	b2CircleShape shape;
	shape.m_radius = radius / BOX2D_SCALE_FACTOR;
	fd.shape = &shape;

	m_body->CreateFixture(&fd);
}

void CircleBody::getRect(Rect& rect, const Vector& position, float angle) const
{
	b2Shape* shape = m_body->GetFixtureList()->GetShape();
	b2CircleShape* circle = dynamic_cast<b2CircleShape*>(shape);
	assert(circle);
	rect.xMin = position.x - circle->m_radius;
	rect.xMax = position.x + circle->m_radius;
	rect.yMin = position.y - circle->m_radius;
	rect.yMax = position.y + circle->m_radius;
}

} // d2d