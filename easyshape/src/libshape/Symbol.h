#ifndef _EASYSHAPE_SYMBOL_H_
#define _EASYSHAPE_SYMBOL_H_

#include "ShapeType.h"

#include <ee/Symbol.h>

#include <vector>

namespace ee { class Shape; }

namespace eshape
{

class Symbol : public ee::Symbol
{
public:
	Symbol();
	Symbol(const Symbol& symbol);
	virtual ~Symbol();

	//
	// IObject interface
	//	
	virtual Symbol* Clone() const;

	//
	// Symbol interface
	//
	virtual void Draw(const s2::RenderParams& params, const ee::Sprite* spr = NULL) const;
	virtual void ReloadTexture() const;
	virtual sm::rect GetSize(const ee::Sprite* sprite = NULL) const;

	void Traverse(ee::Visitor& visitor) const;
	bool Add(ee::Shape* shape);
	bool Remove(ee::Shape* shape);
	bool Clear();

	void SetBG(ee::Symbol* bg);
	const ee::Symbol* GetBG() const { return m_bg; }

	void StoreToFile(const char* filename) const;

	ShapeType GetShapeType() const;

	static ee::Symbol* Create() { return new Symbol(); }

	// todo
	const std::vector<ee::Shape*>& GetShapes() const { return m_shapes; }

protected:
	virtual void LoadResources();

private:
	void LoadBGOutline(ee::Symbol* bg);
	void LoadBGTriStrip(ee::Symbol* bg);

private:
	ee::Symbol* m_bg;
	std::vector<ee::Shape*> m_bg_outline;
	std::vector<std::vector<sm::vec2> > m_bg_tri_strips;

	std::vector<ee::Shape*> m_shapes;

}; // Symbol

}

#endif // _EASYSHAPE_SYMBOL_H_