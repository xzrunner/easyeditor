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
	SymbolContainer(const std::shared_ptr<Symbol>& sym);

	//
	// DataContainer interface
	//
	virtual void Traverse(ee::RefVisitor<ee::Shape>& visitor, bool order = true) const;
	virtual void Traverse(ee::RefVisitor<ee::Shape>& visitor, ee::DataTraverseType type = ee::DT_ALL, bool order = true) const;
	virtual bool Remove(const std::shared_ptr<ee::Shape>& shape);
	virtual bool Insert(const std::shared_ptr<ee::Shape>& shape);
	virtual bool Insert(const std::shared_ptr<ee::Shape>& shape, int idx);
	virtual bool Clear();
	virtual bool ResetOrder(const std::shared_ptr<const ee::Shape>& shape, bool up);
	virtual bool ResetOrderMost(const std::shared_ptr<const ee::Shape>& shape, bool up);

private:
	void LoadShapes(std::vector<std::shared_ptr<ee::Shape>>& shapes) const;
	void StorePolygons(const std::vector<std::shared_ptr<ee::Shape>>& shapes);

private:
	std::shared_ptr<Symbol> m_sym;

}; // SymbolContainer

}

#endif // _EASYTEXTURE_SYMBOL_CONTAINER_H_