#include "BodyData.h"
#include "std_functor.h"
#include "FileHelper.h"
#include "Exception.h"
#include "SymbolFile.h"

#include <sprite2/SymType.h>

#include <json/json.h>

#include <string>

namespace ee
{

BodyData::BodyData()
{
}

BodyData::~BodyData()
{
	BodyDataMgr::Instance()->RemoveItem(m_filename);
	clear();
}

void BodyData::clear()
{
	for_each(m_fixtures.begin(), m_fixtures.end(), DeletePointerFunctor<FixtureData>());
	m_fixtures.clear();
}

bool BodyData::LoadFromFile(const std::string& filename)
{
	if (!FileHelper::IsFileExist(filename)) {
		throw Exception("File: %s don't exist!", filename.c_str());
	}

	clear();

	m_filename = filename;
	int type = SymbolFile::Instance()->Type(filename);
	switch (type)
	{
	case s2::SYM_MESH:
		loadFromMeshFile(filename);
		break;
	case s2::SYM_SHAPE:
		loadFromShapeFile(filename);
		break;
	default:
		return false;
	}

	return true;
}

// todo 去掉MeshSymbol，以后用easymesh的，避免不必要的对easymesh的依赖
void BodyData::loadFromMeshFile(const std::string& filename)
{
	//m_type = e_mesh;

	//MeshSymbol sym;
	//sym.LoadFromFile(filename);

	//const std::vector<sm::vec2>& triangles = sym.getVertices();

	//size_t size = triangles.size() / 3;
	//m_fixtures.reserve(size);
	//size_t index = 0;
	//for (size_t i = 0; i < size; ++i)
	//{
	//	FixtureData* fd = new FixtureData;
	//	fd->vertices.push_back(triangles[index++]);
	//	fd->vertices.push_back(triangles[index++]);
	//	fd->vertices.push_back(triangles[index++]);
	//	m_fixtures.push_back(fd);
	//}
}

// todo 移到libshape中，避免不必要的对libshape的依赖
void BodyData::loadFromShapeFile(const std::string& filename)
{
// 	m_type = e_shapes;
// 
// 	std::vector<Shape*> shapes;
// 	eshape::FileAdapter adapter(shapes);
// 
// 	adapter.load(filename.c_str());
// 	for (size_t i = 0, n = shapes.size();  i< n; ++i)
// 	{
// 		if (ChainShape* chain = dynamic_cast<ChainShape*>(shapes[i]))
// 		{
// 			FixtureData* fd = new FixtureData;
// 			fd->vertices = chain->getVertices();
// 			m_fixtures.push_back(fd);
// 		}
// 		else if (RectShape* rect = dynamic_cast<RectShape*>(shapes[i]))
// 		{
// 			FixtureData* fd = new FixtureData;
// 			fd->vertices.resize(4);
// 			fd->vertices[0] = sm::vec2(rect->m_rect.xmin, rect->m_rect.ymin);
// 			fd->vertices[1] = sm::vec2(rect->m_rect.xmax, rect->m_rect.ymin);
// 			fd->vertices[2] = sm::vec2(rect->m_rect.xmax, rect->m_rect.ymax);
// 			fd->vertices[3] = sm::vec2(rect->m_rect.xmin, rect->m_rect.ymax);
// 			m_fixtures.push_back(fd);
// 		}
// 		else if (CircleShape* circle = dynamic_cast<CircleShape*>(shapes[i]))
// 		{
// 			FixtureData* fd = new FixtureData;
// 			fd->vertices.push_back(sm::vec2(circle->radius*2, circle->radius*2));
// 			m_fixtures.push_back(fd);
// 		}
// 
// 		shapes[i]->RemoveReference();
// 	}
}

}