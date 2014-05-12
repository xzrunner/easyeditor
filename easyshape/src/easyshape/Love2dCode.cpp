#include "Love2dCode.h"
#include "StagePanel.h"
#include "LibraryPanel.h"
#include "LibraryItem.h"

namespace lua = ebuilder::lua;

namespace eshape
{

Love2dCode::Love2dCode(ebuilder::CodeGenerator& gen, LibraryPanel* library)
	: m_gen(gen)
	, m_library(library)
{
}

void Love2dCode::resolve()
{
	{
		std::vector<LibraryItem*> items;
		m_library->getShapePage()->getList()->
			traverse(d2d::FetchAllVisitor<LibraryItem>(items));
		for (size_t i = 0, n = items.size(); i < n; ++i)
			resolve(items[i]);
	}
	{
		std::vector<LibraryItem*> items;
		m_library->getImagePage()->getList()->
			traverse(d2d::FetchAllVisitor<LibraryItem>(items));
		for (size_t i = 0, n = items.size(); i < n; ++i)
			resolve(items[i]);
	}
}

void Love2dCode::resolve(LibraryItem* item)
{
	std::string name = item->getName();

	lua::TableAssign ta(m_gen, name);
	
	std::vector<d2d::IShape*>* shapes 
		= static_cast<std::vector<d2d::IShape*>*>(item->getUserData());
	for (size_t i = 0, n = shapes->size(); i < n; ++i)
		resolve((*shapes)[i]);
}

void Love2dCode::resolve(d2d::IShape* shape)
{
	if (libshape::BezierShape* bezier = dynamic_cast<libshape::BezierShape*>(shape))
		resolve(bezier);
	else if (libshape::PolygonShape* poly = dynamic_cast<libshape::PolygonShape*>(shape))
		resolve(poly);
	else if (libshape::ChainShape* chain = dynamic_cast<libshape::ChainShape*>(shape))
		resolve(chain);
	else if (libshape::RectShape* rect = dynamic_cast<libshape::RectShape*>(shape))
		resolve(rect);
	else if (libshape::CircleShape* circle = dynamic_cast<libshape::CircleShape*>(shape))
		resolve(circle);
}

void Love2dCode::resolve(const libshape::BezierShape* bezier)
{
	std::string type = lua::assign("type", "\"bezier\"");

	std::string name = lua::assign("name", "\""+bezier->name+"\"");

	std::string points = lua::assign("points", "");
	points += "{ ";
	for (size_t i = 0; i < 4; ++i)
	{
		points += wxString::FromDouble(bezier->points[i].x, 1)+",";
		points += wxString::FromDouble(bezier->points[i].y, 1)+",";
	}
	points += "} ";

	lua::tableassign(m_gen, "", 3, type.c_str(), name.c_str(), points.c_str());
}

void Love2dCode::resolve(const libshape::PolygonShape* poly)
{
	std::string type = lua::assign("type", "\"polygon\"");

	std::string name = lua::assign("name", "\""+poly->name+"\"");

	std::string points = lua::assign("points", "");
	const std::vector<d2d::Vector>& vertices = poly->getVertices();
	points += "{ ";
	for (size_t i = 0, n = vertices.size(); i < n; ++i)
	{
		points += wxString::FromDouble(vertices[i].x, 1)+",";
		points += wxString::FromDouble(vertices[i].y, 1)+",";
	}
	points += "} ";

	lua::tableassign(m_gen, "", 3, type.c_str(), name.c_str(), points.c_str());
}

void Love2dCode::resolve(const libshape::ChainShape* chain)
{
	std::string type = lua::assign("type", "\"polygon\"");

	std::string name = lua::assign("name", "\""+chain->name+"\"");

	std::string points = lua::assign("points", "");
	const std::vector<d2d::Vector>& vertices = chain->getVertices();
	points += "{ ";
	for (size_t i = 0, n = vertices.size(); i < n; ++i)
	{
		points += wxString::FromDouble(vertices[i].x, 1)+",";
		points += wxString::FromDouble(vertices[i].y, 1)+",";
	}
	points += "} ";

	std::string closed = chain->isClosed() ? lua::assign("closed", "true") : lua::assign("closed", "false");

	lua::tableassign(m_gen, "", 4, type.c_str(), name.c_str(), points.c_str(), closed.c_str());
}

void Love2dCode::resolve(const libshape::RectShape* rect)
{
	std::string type = lua::assign("type", "\"rect\"");

	std::string name = lua::assign("name", "\""+rect->name+"\"");

	std::string xmin = lua::assign("xmin", wxString::FromDouble(rect->m_rect.xMin, 1).ToStdString());
	std::string xmax = lua::assign("xmax", wxString::FromDouble(rect->m_rect.xMax, 1).ToStdString());
	std::string ymin = lua::assign("ymin", wxString::FromDouble(rect->m_rect.yMin, 1).ToStdString());
	std::string ymax = lua::assign("ymax", wxString::FromDouble(rect->m_rect.yMax, 1).ToStdString());

	lua::tableassign(m_gen, "", 6, type.c_str(), name.c_str(), xmin.c_str(), xmax.c_str(), ymin.c_str(), ymax.c_str());
}

void Love2dCode::resolve(const libshape::CircleShape* circle)
{
	std::string type = lua::assign("type", "\"circle\"");

	std::string name = lua::assign("name", "\""+circle->name+"\"");

	std::string cx = lua::assign("cx", wxString::FromDouble(circle->center.x, 1).ToStdString()),
		cy = lua::assign("cy", wxString::FromDouble(circle->center.y, 1).ToStdString());

	std::string r = lua::assign("r", wxString::FromDouble(circle->radius, 1).ToStdString());

	lua::tableassign(m_gen, "", 5, type.c_str(), name.c_str(), cx.c_str(), cy.c_str(), r.c_str());
}

}