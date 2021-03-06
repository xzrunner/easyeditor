#include "ShapeBuilder.h"
#include "PackShape.h"
#include "PackUI.h"
#include "PackTag.h"

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
	auto itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		delete itr->second;
	}
}

void ShapeBuilder::Traverse(ee::Visitor<IPackNode>& visitor) const
{
	auto itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<PackShape*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
	}
}

bool ShapeBuilder::CanHandle(const std::shared_ptr<etexture::Symbol>& sym) const
{
	auto& polys = sym->GetPolygons();
	if (polys.size() != 1) {
		return false;
	}
	eshape::PolygonShape* poly = dynamic_cast<eshape::PolygonShape*>(polys[0].get());
	if (!poly) {
		return false;
	}
	const eshape::ColorMaterial* material = dynamic_cast<const eshape::ColorMaterial*>(poly->GetMaterial());
	if (!material) {
		return false;
	}
	return true;
}

const IPackNode* ShapeBuilder::Create(const std::shared_ptr<etexture::Symbol>& sym)
{
	auto itr = m_map_data.find(sym);
	if (itr != m_map_data.end()) {
		return itr->second;
	}

	PackShape* node = new PackShape;
	Load(sym, node);
	m_map_data.insert(std::make_pair(sym, node));

	PackUI::Instance()->OnKnownPackID(sym->GetFilepath(), node->GetSprID());
	PackTag::Instance()->OnKnownPackID(sym->GetFilepath(), node->GetSprID());	

	return node;
}

void ShapeBuilder::Load(const std::shared_ptr<etexture::Symbol>& sym, PackShape* shape)
{
	auto& polys = sym->GetPolygons();
	if (polys.size() != 1) {
		throw ee::Exception("ShapeBuilder::Load shapes.size(): %d filepath: %s", polys.size(), sym->GetFilepath().c_str());
	}
	eshape::PolygonShape* poly = dynamic_cast<eshape::PolygonShape*>(polys[0].get());
	if (!poly) {
		throw ee::Exception("ShapeBuilder::Load !poly, filepath: %s", sym->GetFilepath().c_str());
	}
	const eshape::ColorMaterial* material = dynamic_cast<const eshape::ColorMaterial*>(poly->GetMaterial());
	if (!material) {
		throw ee::Exception("ShapeBuilder::Load !material, filepath: %s", sym->GetFilepath().c_str());
	}

	auto& vertices = material->GetTriangles();
	if (vertices.size() % 3 != 0) {
		throw ee::Exception("ShapeBuilder::Load err meaterial, filepath: %s", sym->GetFilepath().c_str());
	}

	shape->type = ST_TRIANGLES;
	shape->color = material->GetColor();
	shape->vertices = vertices;
}

}