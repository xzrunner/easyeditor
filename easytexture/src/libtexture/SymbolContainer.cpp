#include "SymbolContainer.h"
#include "Symbol.h"

#include <ee/ObjectVector.h>
#include <ee/Shape.h>

#include <easyshape.h>

#include <sprite2/PolygonShape.h>

namespace etexture
{

SymbolContainer::SymbolContainer(Symbol* sym)
{
	m_sym = sym;
	m_sym->AddReference();
}

SymbolContainer::~SymbolContainer()
{
	m_sym->RemoveReference();
}

void SymbolContainer::Traverse(ee::Visitor<ee::Shape>& visitor, bool order/* = true*/) const
{
	std::vector<ee::Shape*> shapes;
	LoadShapes(shapes);
	ee::ObjectVector<ee::Shape>::Traverse(shapes, visitor, order);
}

void SymbolContainer::Traverse(ee::Visitor<ee::Shape>& visitor, ee::DataTraverseType type, bool order) const
{
	Traverse(visitor, order);
}

bool SymbolContainer::Remove(ee::Shape* shape)
{
	std::vector<ee::Shape*> shapes;
	LoadShapes(shapes);
	bool ret = ee::ObjectVector<ee::Shape>::Remove(shapes, shape);
	StorePolygons(shapes);
	return ret;
}

bool SymbolContainer::Insert(ee::Shape* shape)
{
	std::vector<ee::Shape*> shapes;
	LoadShapes(shapes);
	bool ret = ee::ObjectVector<ee::Shape>::Insert(shapes, shape);
	StorePolygons(shapes);
	return ret;
}

bool SymbolContainer::Insert(ee::Shape* shape, int idx)
{
	std::vector<ee::Shape*> shapes;
	LoadShapes(shapes);
	bool ret = ee::ObjectVector<ee::Shape>::Insert(shapes, shape, idx);
	StorePolygons(shapes);
	return ret;
}

bool SymbolContainer::Clear()
{
	std::vector<ee::Shape*> shapes;
	LoadShapes(shapes);
	bool ret = ee::ObjectVector<ee::Shape>::Clear(shapes);
	m_sym->GetPolygons().clear();
	return ret;
}

bool SymbolContainer::ResetOrder(const ee::Shape* shape, bool up)
{
	std::vector<ee::Shape*> shapes;
	LoadShapes(shapes);
	bool ret = ee::ObjectVector<ee::Shape>::ResetOrder(shapes, shape, up);
	StorePolygons(shapes);
	return ret;
}

bool SymbolContainer::ResetOrderMost(const ee::Shape* shape, bool up)
{
	std::vector<ee::Shape*> shapes;
	LoadShapes(shapes);
	bool ret = ee::ObjectVector<ee::Shape>::ResetOrderMost(shapes, shape, up);
	StorePolygons(shapes);
	return ret;
}

void SymbolContainer::LoadShapes(std::vector<ee::Shape*>& shapes) const
{
	std::vector<s2::PolygonShape*>& polygons = m_sym->GetPolygons();
	shapes.reserve(polygons.size());
	for (int i = 0, n = polygons.size(); i < n; ++i) {
		shapes.push_back(dynamic_cast<ee::Shape*>(static_cast<s2::Shape*>(polygons[i])));
	}
}

void SymbolContainer::StorePolygons(const std::vector<ee::Shape*>& shapes)
{
	std::vector<s2::PolygonShape*>& polygons = m_sym->GetPolygons();
	polygons.clear();
	polygons.reserve(shapes.size());
	for (int i = 0, n = shapes.size(); i < n; ++i) {
		polygons.push_back(static_cast<eshape::PolygonShape*>(shapes[i]));
	}
}

}