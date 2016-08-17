#ifndef _EASYCOMPLEX_SYMBOL_CONTAINER_H_
#define _EASYCOMPLEX_SYMBOL_CONTAINER_H_

#include <ee/SprDataContainer.h>

#include <vector>

namespace ecomplex
{

class Symbol;

class SymbolContainer : public ee::SprDataContainer
{
public:
	SymbolContainer(Symbol* sym);
	virtual ~SymbolContainer();

	//
	// SprDataContainer interface
	//
	virtual void Traverse(ee::Visitor<ee::Sprite>& visitor, bool order = true) const;
	virtual void Traverse(ee::Visitor<ee::Sprite>& visitor, ee::DataTraverseType type = ee::DT_ALL, bool order = true) const;
	virtual bool Remove(ee::Sprite* obj);
	virtual bool Insert(ee::Sprite* obj);
	virtual bool Insert(ee::Sprite* obj, int idx);
	virtual bool Clear();
	virtual bool ResetOrder(const ee::Sprite* obj, bool up);
	virtual bool ResetOrderMost(const ee::Sprite* obj, bool up);
	virtual bool Sort(std::vector<ee::Sprite*>& sprs);

private:
	void GetSprites(std::vector<ee::Sprite*>& sprs) const;

private:
	Symbol* m_sym;

}; // SymbolContainer

}

#endif // _EASYCOMPLEX_SYMBOL_CONTAINER_H_