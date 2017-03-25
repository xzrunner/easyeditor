#ifndef _EASYSHAPE_SYMBOL_H_
#define _EASYSHAPE_SYMBOL_H_

#include "ShapeType.h"

#include <ee/Symbol.h>

#include <sprite2/ShapeSymbol.h>

#include <vector>

namespace ee { class Shape; }

namespace eshape
{

class Symbol : public ee::Symbol, public s2::ShapeSymbol
{
public:
	Symbol();
	virtual ~Symbol();

	/**
	 *  @interface
	 *    s2::Symbol
	 */
	virtual void Draw(const s2::RenderParams& params, const s2::Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const s2::Sprite* spr = NULL, const s2::Actor* actor = NULL) const;

	/**
	 *  @interface
	 *    ee::Symbol
	 */
	virtual void ReloadTexture() const;

	void Traverse(ee::Visitor<ee::Shape>& visitor) const;
	bool Add(ee::Shape* shape);
	bool Remove(ee::Shape* shape);
	bool Clear();

	void SetBG(ee::Symbol* bg);
	const ee::Symbol* GetBG() const { return m_bg; }

	void StoreToFile(const char* filename) const;

	ShapeType GetShapeType() const;

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual bool LoadResources();

private:
	void LoadBGOutline(ee::Symbol* bg);
	void LoadBGTriStrip(ee::Symbol* bg);

private:
	ee::Symbol* m_bg;

	std::vector<ee::Shape*> m_bg_outline;
	std::vector<std::vector<sm::vec2> > m_bg_tri_strips;

}; // Symbol

}

#endif // _EASYSHAPE_SYMBOL_H_