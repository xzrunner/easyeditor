#ifndef _EASYCOMPLEX_SYMBOL_CONTAINER_H_
#define _EASYCOMPLEX_SYMBOL_CONTAINER_H_

#include <ee/DataContainer.h>

#include <vector>

namespace ee { class Sprite; }

namespace ecomplex
{

class Symbol;

class SymbolContainer : public ee::DataContainer
{
public:
	SymbolContainer(Symbol* symbol);
	virtual ~SymbolContainer();

	//
	// DataContainer interface
	//
	virtual void Traverse(ee::Visitor& visitor, bool order = true) const;
	virtual void Traverse(ee::Visitor& visitor, ee::DataTraverseType type = ee::DT_ALL, bool order = true) const;
	virtual bool Remove(Object* obj);
	virtual bool Insert(Object* obj);
	virtual bool Insert(Object* obj, int idx);
	virtual bool Clear();
	virtual bool ResetOrder(const Object* obj, bool up);
	virtual bool ResetOrderMost(const Object* obj, bool up);

private:
	void GetSprites(std::vector<ee::Sprite*>& sprites) const;

private:
	Symbol* m_symbol;

}; // SymbolContainer

}

#endif // _EASYCOMPLEX_SYMBOL_CONTAINER_H_