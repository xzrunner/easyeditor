#include "PolygonBody.h"
#include "BodyData.h"
#include "physics_const.h"
#include "Rect.h"
#include "Math2D.h"

#include <Box2D/Box2D.h>

namespace ee
{

PolygonBody::PolygonBody(b2World* world, const BodyData& bodyData, float scale/* = 1.0f*/)
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

PolygonBody::PolygonBody(b2World* world, float hWidth, float hHeight, 
						 bool isStatic, float scale/* = 1.0f*/)
	: IBody(world, scale)
{	
	b2BodyDef bd;
	bd.type = isStatic ? b2_staticBody : b2_dynamicBody;
	m_body = world->CreateBody(&bd);

	b2PolygonShape shape;
	shape.SetAsBox(hWidth * scale / BOX2D_SCALE_FACTOR, hHeight * scale / BOX2D_SCALE_FACTOR);

 	b2FixtureDef fd;
 	fd.shape = &shape;
 	fd.density = 1.0f;
 	fd.friction = 1.0f;

	m_body->CreateFixture(&fd);
}

void PolygonBody::GetRect(Rect& rect, const sm::vec2& position, float angle) const
{
	rect.MakeInfinite();
	for (b2Fixture* f = m_body->GetFixtureList(); f; f = f->GetNext())
	{
		b2PolygonShape* poly = dynamic_cast<b2PolygonShape*>(f->GetShape());
		assert(poly);
		for (int i = 0; i < poly->GetVertexCount(); ++i)
		{
			const b2Vec2& p = poly->m_vertices[i];
			sm::vec2 transform = Math2D::RotateVector(sm::vec2(p.x, p.y), angle) + position;
			rect.Combine(transform);
		}
	}
}

}
