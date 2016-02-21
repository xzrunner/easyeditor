#ifndef _EASYEDITOR_BODY_FACTORY_H_
#define _EASYEDITOR_BODY_FACTORY_H_

#include "FixtureData.h"

#include <vector>

class b2World;

namespace ee
{

class IBody;

class BodyFactory
{
public:
	static IBody* createBody(const std::string& filename, float scale = 1.0f);
	static IBody* createBody(const std::vector<Vector>& triangles, float scale = 1.0f);
	static IBody* createBody(const std::vector<FixtureDataInfo>& fixtures, float scale = 1.0f);

	static void setWorld(b2World* world);

private:
	static b2World* m_world;

}; // BodyFactory

inline void BodyFactory::setWorld(b2World* world)
{
	m_world = world;
}

}

#endif // _EASYEDITOR_BODY_FACTORY_H_