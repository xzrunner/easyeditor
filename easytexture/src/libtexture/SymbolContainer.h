#ifndef _EASYTEXTURE_SYMBOL_CONTAINER_H_
#define _EASYTEXTURE_SYMBOL_CONTAINER_H_

#include <ee/DataContainer.h>
#include <ee/Shape.h>

namespace etexture
{

class Symbol;

class SymbolContainer : public ee::DataContainer<ee::Shape>
{
public:
	SymbolContainer(Symbol* sym);
	virtual ~SymbolContainer();

	//
	// DataContainer interface
	//
	virtual void Traverse(ee::Visitor<ee::Shape>& visitor, bool order = true) const;
	virtual void Traverse(ee::Visitor<ee::Shape>& visitor, ee::DataTraverseType type = ee::DT_ALL, bool order = true) const;
	virtual bool Remove(ee::Shape* shape);
	virtual bool Insert(ee::Shape* shape);
	virtual bool Insert(ee::Shape* shape, int idx);
	virtual bool Clear();
	virtual bool ResetOrder(const ee::Shape* shape, bool up);
	virtual bool ResetOrderMost(const ee::Shape* shape, bool up);

private:
	void LoadShapes(std::vector<ee::Shape*>& shapes) const;
	void StorePolygons(const std::vector<ee::Shape*>& shapes);

private:
	Symbol* m_sym;

}; // SymbolContainer

}

#endif // _EASYTEXTURE_SYMBOL_CONTAINER_H_