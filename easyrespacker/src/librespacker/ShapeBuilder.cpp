#include "ShapeBuilder.h"
#include "PackShape.h"

#include <ee/Exception.h>

#include <easytexture.h>
#include <easyshape.h>

namespace erespacker
{

ShapeBuilder::ShapeBuilder()
{
}

ShapeBuilder::~ShapeBuilder()
{
	std::map<const etexture::Symbol*, const PackShape*>::iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		delete itr->second;
	}
}

void ShapeBuilder::Traverse(ee::Visitor& visitor) const
{
	std::map<const etexture::Symbol*, const PackShape*>::const_iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<PackShape*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
	}
}

bool ShapeBuilder::CanHandle(const etexture::Symbol* symbol) const
{
	const std::vector<ee::Shape*>& shapes = symbol->GetAllShapes();
	if (shapes.size() != 1) {
		return false;
	}
	eshape::PolygonShape* poly = dynamic_cast<eshape::PolygonShape*>(shapes[0]);
	if (!poly) {
		return false;
	}
	const eshape::ColorMaterial* material = dynamic_cast<const eshape::ColorMaterial*>(poly->GetMaterial());
	if (!material) {
		return false;
	}
	return true;
}

const IPackNode* ShapeBuilder::Create(const etexture::Symbol* symbol)
{
	std::map<const etexture::Symbol*, const PackShape*>::iterator 
		itr = m_map_data.find(symbol);
	if (itr != m_map_data.end()) {
		return itr->second;
	}

	PackShape* node = new PackShape;
	Load(symbol, node);
	m_map_data.insert(std::make_pair(symbol, node));
	return node;
}

void ShapeBuilder::Load(const etexture::Symbol* symbol, PackShape* shape)
{
	const std::vector<ee::Shape*>& shapes = symbol->GetAllShapes();
	if (shapes.size() != 1) {
		throw ee::Exception("ShapeBuilder::Load shapes.size(): %d filepath: %s", shapes.size(), symbol->GetFilepath().c_str());
	}
	eshape::PolygonShape* poly = dynamic_cast<eshape::PolygonShape*>(shapes[0]);
	if (!poly) {
		throw ee::Exception("ShapeBuilder::Load !poly, filepath: %s", symbol->GetFilepath().c_str());
	}
	const eshape::ColorMaterial* material = dynamic_cast<const eshape::ColorMaterial*>(poly->GetMaterial());
	if (!material) {
		throw ee::Exception("ShapeBuilder::Load !material, filepath: %s", symbol->GetFilepath().c_str());
	}

	const std::vector<sm::vec2>& vertices = material->GetTriangles();
	if (vertices.size() % 3 != 0) {
		throw ee::Exception("ShapeBuilder::Load err meaterial, filepath: %s", symbol->GetFilepath().c_str());
	}

	shape->type = ST_TRIANGLES;
	shape->color = material->GetColor();
	shape->vertices = vertices;
}

}