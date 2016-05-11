#ifndef _EASYTEXTURE_SYMBOL_H_
#define _EASYTEXTURE_SYMBOL_H_

#include <ee/Symbol.h>

#include <vector>

namespace ee { class Shape; }

namespace etexture
{

class Symbol : public ee::Symbol
{
public:
	Symbol();
	Symbol(const Symbol& s);
	virtual ~Symbol();

	//
	// Cloneable interface
	//
	virtual Symbol* Clone() const;

	//
	// Symbol interfaces
	//
	virtual void Draw(const s2::RenderParams& trans, const s2::Sprite* spr = NULL) const;
	virtual void ReloadTexture() const;
	virtual ee::Rect GetSize(const ee::Sprite* sprite = NULL) const;

	const std::vector<ee::Shape*>& GetAllShapes() const { return m_shapes; }

	static ee::Symbol* Create() { return new Symbol(); }
	
protected:
	virtual void LoadResources();

	void Clear();

private:
	std::vector<ee::Shape*> m_shapes;

	friend class SymbolContainer;

}; // Symbol

}

#endif // _EASYTEXTURE_SYMBOL_H_