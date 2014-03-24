#include <json/json.h>

#include <easyshape.h>

#include "BodyData.h"
#include "ChainShape.h"

#include "common/tools.h"
#include "common/FileNameParser.h"
#include "common/PolylineFileAdapter.h"
#include "common/CircleFileAdapter.h"
#include "common/EShapeFileAdapter.h"
#include "dataset/MeshSymbol.h"
#include "dataset/CircleShape.h"
#include "dataset/RectShape.h"

namespace d2d
{

BodyData::BodyData()
{
}

BodyData::~BodyData()
{
	BodyDataMgr::Instance()->removeItem(m_filename);
	clear();
}

void BodyData::clear()
{
	for_each(m_fixtures.begin(), m_fixtures.end(), DeletePointerFunctor<FixtureData>());
	m_fixtures.clear();
}

bool BodyData::loadFromFile(const wxString& filename)
{
	clear();

	m_filename = filename;

	FileNameParser::Type type = FileNameParser::getFileType(filename);
	switch (type)
	{
	case FileNameParser::e_mesh:
		loadFromMeshFile(filename);
		break;
	case FileNameParser::e_polyline:
		loadFromPolygonFile(filename);
		break;
	case FileNameParser::e_circle:
		loadFromCircleFile(filename);
		break;
	case FileNameParser::e_shape:
		loadFromShapeFile(filename);
	default:
		return false;
	}

	return true;
}

void BodyData::loadFromMeshFile(const wxString& filename)
{
	m_type = e_mesh;

	MeshSymbol symbol;
	symbol.loadFromFile(filename);

	const std::vector<Vector>& triangles = symbol.getVertices();

	size_t size = triangles.size() / 3;
	m_fixtures.reserve(size);
	size_t index = 0;
	for (size_t i = 0; i < size; ++i)
	{
		FixtureData* fd = new FixtureData;
		fd->vertices.push_back(triangles[index++]);
		fd->vertices.push_back(triangles[index++]);
		fd->vertices.push_back(triangles[index++]);
		m_fixtures.push_back(fd);
	}
}

void BodyData::loadFromPolygonFile(const wxString& filename)
{
	m_type = e_polygon;

	std::vector<ChainShape*> chains;
	PolylineFileAdapter fileAdapter(chains);
	fileAdapter.load(filename.c_str());

	m_fixtures.reserve(fileAdapter.m_chains.size());
	for (size_t i = 0, n = fileAdapter.m_chains.size(); i < n; ++i)
	{
		FixtureData* fd = new FixtureData;
		fd->vertices = fileAdapter.m_chains[i]->getVertices();
		m_fixtures.push_back(fd);
	}

	for_each(chains.begin(), chains.end(), DeletePointerFunctor<ChainShape>());
}

void BodyData::loadFromCircleFile(const wxString& filename)
{
	m_type = e_circle;

	CircleFileAdapter fa;
	fa.load(filename);

	FixtureData* fd = new FixtureData;
	fd->vertices.push_back(Vector(fa.m_width, fa.m_height));
	m_fixtures.push_back(fd);
}

void BodyData::loadFromShapeFile(const wxString& filename)
{
	m_type = e_shapes;

	std::vector<IShape*> shapes;
	libshape::FileAdapter adapter(shapes);

	adapter.load(filename.c_str());
	for (size_t i = 0, n = shapes.size();  i< n; ++i)
	{
		if (ChainShape* chain = dynamic_cast<ChainShape*>(shapes[i]))
		{
			FixtureData* fd = new FixtureData;
			fd->vertices = chain->getVertices();
			m_fixtures.push_back(fd);
		}
		else if (RectShape* rect = dynamic_cast<RectShape*>(shapes[i]))
		{
			FixtureData* fd = new FixtureData;
			fd->vertices.resize(4);
			fd->vertices[0] = d2d::Vector(rect->m_rect.xMin, rect->m_rect.yMin);
			fd->vertices[1] = d2d::Vector(rect->m_rect.xMax, rect->m_rect.yMin);
			fd->vertices[2] = d2d::Vector(rect->m_rect.xMax, rect->m_rect.yMax);
			fd->vertices[3] = d2d::Vector(rect->m_rect.xMin, rect->m_rect.yMax);
			m_fixtures.push_back(fd);
		}
		else if (CircleShape* circle = dynamic_cast<CircleShape*>(shapes[i]))
		{
			FixtureData* fd = new FixtureData;
			fd->vertices.push_back(Vector(circle->radius*2, circle->radius*2));
			m_fixtures.push_back(fd);
		}

		shapes[i]->release();
	}
}
} // d2d