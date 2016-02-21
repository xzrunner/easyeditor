#include "ShapesBody.h"
#include "BodyData.h"
#include "physics_const.h"
#include "Rect.h"
#include "Math2D.h"

#include <Box2D/Box2D.h>

namespace ee
{

ShapesBody::ShapesBody(b2World* world, const BodyData& bodyData, float scale/* = 1.0f*/)
	: IBody(world, scale)
{
	m_filename = bodyData.getFilename();

	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	m_body = world->CreateBody(&bd);

	const std::vector<BodyData::FixtureData*>& fixturesData = bodyData.getAllFixturesData();
	for (size_t i = 0, n = fixturesData.size(); i < n; ++i)
	{
		const BodyData::FixtureData* fixtureData = fixturesData[i];

		const size_t size = fixtureData->vertices.size();
		if (size == 1)
		{
			b2FixtureDef fd;
			fd.density = 1;
			fd.restitution = 0.6f;

			b2CircleShape shape;
			shape.m_radius = fixtureData->vertices[0].x * 0.5f * scale / BOX2D_SCALE_FACTOR;
			fd.shape = &shape;

			m_body->CreateFixture(&fd);
		}
		else
		{
			std::vector<b2Vec2> transList(size);
			for (size_t j = 0, m = size; j < m; ++j)
			{
				transList[j].x = fixtureData->vertices[j].x * scale / BOX2D_SCALE_FACTOR;
				transList[j].y = fixtureData->vertices[j].y * scale / BOX2D_SCALE_FACTOR;
			}

			b2PolygonShape shape;
			shape.Set(&transList[0], size);

			b2FixtureDef fd;
			fd.shape = &shape;
			fd.friction = 0.5f;
			fixtureData->setFixtureDef(fd);

			m_body->CreateFixture(&fd);
		}
	}
}

void ShapesBody::GetRect(Rect& rect, const Vector& position, float angle) const
{
	rect.MakeInfinite();
	for (b2Fixture* f = m_body->GetFixtureList(); f; f = f->GetNext())
	{
		if (b2PolygonShape* poly = dynamic_cast<b2PolygonShape*>(f->GetShape()))
		{
			assert(poly);
			for (int i = 0; i < poly->GetVertexCount(); ++i)
			{
				const b2Vec2& p = poly->m_vertices[i];
				Vector transform = Math2D::RotateVector(Vector(p.x, p.y), angle) + position;
				rect.Combine(transform);
			}
		}
		else if (b2Shape* shape = m_body->GetFixtureList()->GetShape())
		{
			b2CircleShape* circle = dynamic_cast<b2CircleShape*>(shape);
			assert(circle);
			rect.Combine(Vector(position.x - circle->m_radius, position.y - circle->m_radius));
			rect.Combine(Vector(position.x + circle->m_radius, position.y + circle->m_radius));
		}
	}
}

}
