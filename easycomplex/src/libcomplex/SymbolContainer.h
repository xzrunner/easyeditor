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
	SymbolContainer(std::shared_ptr<Symbol>& sym);

	//
	// SprDataContainer interface
	//
	virtual void Traverse(ee::RefVisitor<ee::Sprite>& visitor, bool order = true) const override;
	virtual void Traverse(ee::RefVisitor<ee::Sprite>& visitor, ee::DataTraverseType type = ee::DT_ALL, bool order = true) const override;
	virtual bool Remove(const ee::SprPtr& obj) override;
	virtual bool Insert(const ee::SprPtr& obj) override;
	virtual bool Insert(const ee::SprPtr& obj, int idx) override;
	virtual bool Clear() override;
	virtual bool ResetOrder(const ee::SprConstPtr& obj, bool up) override;
	virtual bool ResetOrderMost(const ee::SprConstPtr& obj, bool up) override;
	virtual bool Sort(std::vector<ee::SprPtr>& sprs) override;

private:
	void GetSprites(std::vector<ee::SprPtr>& sprs) const;

private:
	std::shared_ptr<Symbol>& m_sym;

}; // SymbolContainer

}

#endif // _EASYCOMPLEX_SYMBOL_CONTAINER_H_