#include "PackShape.h"
#include "PackNodeFactory.h"
#include "binary_io.h"
#include "PackVertices.h"

#include <easyshape.h>
#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <sprite2/Shape.h>
#include <simp/NodeShape.h>
#include <simp/simp_types.h>

namespace esprpacker
{

PackShape::PackShape(const eshape::Symbol* sym)
	: m_color(0)
	, m_filling(false)
{
	const s2::Shape* shape = sym->GetShape();
	if (const s2::PointShape* point = dynamic_cast<const s2::PointShape*>(shape))
	{
		m_type = gum::POINT;
		m_vertices.push_back(point->GetPos());
	}
	else if (const s2::RectShape* rect = dynamic_cast<const s2::RectShape*>(shape))
	{
		m_type = gum::RECT;
		const sm::rect& r = rect->GetRect();
		m_vertices.push_back(sm::vec2(r.xmin, r.ymin));
		m_vertices.push_back(sm::vec2(r.xmax, r.ymax));
	}
	else if (const s2::CircleShape* circle = dynamic_cast<const s2::CircleShape*>(shape))
	{
		m_type = gum::CIRCLE;
		m_vertices.push_back(circle->GetCenter());
		m_vertices.push_back(sm::vec2(circle->GetRadius(), circle->GetRadius()));
	}
	else if (const s2::PolygonShape* polygon = dynamic_cast<const s2::PolygonShape*>(shape))
	{
		m_type = gum::POLYGON;
		m_filling = true;
		m_vertices = polygon->GetVertices();
		const s2::Polygon* p = polygon->GetPolygon();
		if (const s2::ColorPolygon* cp = dynamic_cast<const s2::ColorPolygon*>(p)) {
			m_color = cp->GetColor();
		}
	}
	else if (const s2::PolylineShape* polyline = dynamic_cast<const s2::PolylineShape*>(shape))
	{
		m_type = gum::POLYLINE;
		m_vertices = polyline->GetVertices();
	}
}

void PackShape::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, float scale) const
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + GetFilepath());

	lua::assign_with_end(gen, "type", "\"shape\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(m_id));

	lua::assign_with_end(gen, "shape_type", m_type);
	lua::assign_with_end(gen, "color", m_color.ToRGBA());

	PackVertices::PackToLua(gen, m_vertices, "vertices");

	gen.detab();
	gen.line("},");
}

int PackShape::SizeOfUnpackFromBin() const
{
	int sz = simp::NodeShape::Size();
	sz += PackVertices::SizeOfUnpackFromBin(m_vertices);
	return sz;
}

int PackShape::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);								// id
	sz += sizeof(uint8_t);								// type
	sz += sizeof(uint8_t);								// shape type
	sz += sizeof(uint32_t);								// color
	sz += PackVertices::SizeOfPackToBin(m_vertices);	// vertices
	return sz;
}

void PackShape::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{
	uint32_t id = m_id;
	pack(id, ptr);

	uint8_t type = simp::TYPE_SHAPE;
	pack(type, ptr);

	uint8_t shape_type = m_type;
	pack(shape_type, ptr);

	uint32_t font_color = m_color.ToRGBA();
	pack(font_color, ptr);

	PackVertices::PackToBin(m_vertices, ptr);
}

}