#include "ChainBody.h"
#include "BodyData.h"
#include "const.h"


#include <Box2D/Box2D.h>

namespace ephysics
{

ChainBody::ChainBody(b2World* world, const BodyData& bodyData, float scale/* = 1.0f*/)
	: IBody(world, scale)
{
	m_filename = bodyData.getFilename();

	b2BodyDef bd;
	bd.type = b2_staticBody;
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

		b2ChainShape shape;
		shape.CreateChain(&transList[0], size);

		b2FixtureDef fd;
		fd.shape = &shape;
		fixtureData->setFixtureDef(fd);

		m_body->CreateFixture(&fd);
	}
}

ChainBody::ChainBody(b2World* world, const std::vector<ee::Vector>& vertices, bool isLoop, float scale/* = 1.0f*/)
	: IBody(world, scale)
{
	b2BodyDef bd;
	bd.type = b2_staticBody;
	m_body = world->CreateBody(&bd);

	b2ChainShape shape;
	const size_t size = vertices.size();
	std::vector<b2Vec2> transList(size);
	for (size_t i = 0; i < size; ++i)
	{
		transList[i].x = vertices[i].x * scale / BOX2D_SCALE_FACTOR;
		transList[i].y = vertices[i].y * scale / BOX2D_SCALE_FACTOR;
	}
	if (isLoop) shape.CreateLoop(&transList[0], size);
	else shape.CreateChain(&transList[0], size);

	m_body->CreateFixture(&shape, 1.0f);
}

ChainBody::ChainBody(b2World* world, float scale/* = 1.0f*/)
	: IBody(world, scale)
{
	b2BodyDef bd;
	bd.type = b2_staticBody;
	m_body = world->CreateBody(&bd);
}
 
void ChainBody::getRect(ee::Rect& rect, const ee::Vector& position, float angle) const
{
	rect.MakeInfinite();
	for (b2Fixture* f = m_body->GetFixtureList(); f; f = f->GetNext())
	{
		b2ChainShape* chain = dynamic_cast<b2ChainShape*>(f->GetShape());
		assert(chain);
		for (size_t i = 0; i < chain->m_count; ++i)
		{
			const b2Vec2& p = chain->m_vertices[i];
			ee::Vector transform = ee::Math2D::RotateVector(ee::Vector(p.x, p.y), angle) + position;
			rect.Combine(transform);
		}
	}
}

}