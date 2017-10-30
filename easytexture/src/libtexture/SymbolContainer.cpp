#include "SymbolContainer.h"
#include "Symbol.h"

#include <ee/ObjectVector.h>
#include <ee/Shape.h>

#include <easyshape.h>

#include <sprite2/PolygonShape.h>

namespace etexture
{

SymbolContainer::SymbolContainer(const std::shared_ptr<Symbol>& sym)
	: m_sym(sym)
{
}

void SymbolContainer::Traverse(ee::RefVisitor<ee::Shape>& visitor, bool order/* = true*/) const
{
	std::vector<std::shared_ptr<ee::Shape>> shapes;
	LoadShapes(shapes);
	ee::ObjectVector<ee::Shape>::Traverse(shapes, visitor, order);
}

void SymbolContainer::Traverse(ee::RefVisitor<ee::Shape>& visitor, ee::DataTraverseType type, bool order) const
{
	Traverse(visitor, order);
}

bool SymbolContainer::Remove(const std::shared_ptr<ee::Shape>& shape)
{
	std::vector<std::shared_ptr<ee::Shape>> shapes;
	LoadShapes(shapes);
	bool ret = ee::ObjectVector<ee::Shape>::Remove(shapes, shape);
	StorePolygons(shapes);
	return ret;
}

bool SymbolContainer::Insert(const std::shared_ptr<ee::Shape>& shape)
{
	std::vector<std::shared_ptr<ee::Shape>> shapes;
	LoadShapes(shapes);
	bool ret = ee::ObjectVector<ee::Shape>::Insert(shapes, shape);
	StorePolygons(shapes);
	return ret;
}

bool SymbolContainer::Insert(const std::shared_ptr<ee::Shape>& shape, int idx)
{
	std::vector<std::shared_ptr<ee::Shape>> shapes;
	LoadShapes(shapes);
	bool ret = ee::ObjectVector<ee::Shape>::Insert(shapes, shape, idx);
	StorePolygons(shapes);
	return ret;
}

bool SymbolContainer::Clear()
{
	std::vector<std::shared_ptr<ee::Shape>> shapes;
	LoadShapes(shapes);
	bool ret = ee::ObjectVector<ee::Shape>::Clear(shapes);
	m_sym->GetPolygons().clear();
	return ret;
}

bool SymbolContainer::ResetOrder(const std::shared_ptr<const ee::Shape>& shape, bool up)
{
	std::vector<std::shared_ptr<ee::Shape>> shapes;
	LoadShapes(shapes);
	bool ret = ee::ObjectVector<ee::Shape>::ResetOrder(shapes, shape, up);
	StorePolygons(shapes);
	return ret;
}

bool SymbolContainer::ResetOrderMost(const std::shared_ptr<const ee::Shape>& shape, bool up)
{
	std::vector<std::shared_ptr<ee::Shape>> shapes;
	LoadShapes(shapes);
	bool ret = ee::ObjectVector<ee::Shape>::ResetOrderMost(shapes, shape, up);
	StorePolygons(shapes);
	return ret;
}

void SymbolContainer::LoadShapes(std::vector<std::shared_ptr<ee::Shape>>& shapes) const
{
	auto& polygons = m_sym->GetPolygons();
	shapes.reserve(polygons.size());
	for (int i = 0, n = polygons.size(); i < n; ++i) {
		shapes.push_back(std::dynamic_pointer_cast<ee::Shape>(polygons[i]));
	}
}

void SymbolContainer::StorePolygons(const std::vector<std::shared_ptr<ee::Shape>>& shapes)
{
	auto& polygons = m_sym->GetPolygons();
	polygons.clear();
	polygons.reserve(shapes.size());
	for (int i = 0, n = shapes.size(); i < n; ++i) {
		polygons.push_back(std::dynamic_pointer_cast<eshape::PolygonShape>(shapes[i]));
	}
}

}