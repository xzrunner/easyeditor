#ifndef _EASYSHAPE_SYMBOL_H_
#define _EASYSHAPE_SYMBOL_H_

#include "ShapeType.h"

#include <ee/Symbol.h>
#include <ee/Shape.h>

#include <cu/cu_stl.h>
#include <sprite2/ShapeSymbol.h>

#include <vector>

namespace eshape
{

class Symbol : public ee::Symbol, public s2::ShapeSymbol
{
public:
	Symbol();

	/**
	 *  @interface
	 *    s2::Symbol
	 */
	virtual s2::RenderReturn DrawTree(cooking::DisplayList* dlist, const s2::RenderParams& rp, const s2::Sprite* spr = nullptr) const override;

	/**
	 *  @interface
	 *    ee::Symbol
	 */
	virtual void ReloadTexture() const override;

	void Traverse(ee::RefVisitor<ee::Shape>& visitor) const;
	bool Add(ee::ShapePtr& shape);
	bool Remove(const ee::ShapePtr& shape);
	bool Clear();

	void SetBG(const ee::SymPtr& bg);
	auto& GetBG() const { return m_bg; }

	void StoreToFile(const char* filename) const;

	ShapeType GetShapeType() const;

	static ee::SymPtr Create() { return std::make_shared<Symbol>(); }

protected:
//	virtual sm::rect GetBoundingImpl(const s2::Sprite* spr = NULL, const s2::Actor* actor = NULL, bool cache = true) const override;

	virtual bool LoadResources() override;

private:
	void LoadBGOutline(const ee::SymPtr& bg);
	void LoadBGTriStrip(const ee::SymPtr& bg);

private:
	ee::SymPtr m_bg;

	CU_VEC<ee::ShapePtr> m_bg_outline;
	CU_VEC<CU_VEC<sm::vec2>> m_bg_tri_strips;

}; // Symbol

}

#endif // _EASYSHAPE_SYMBOL_H_