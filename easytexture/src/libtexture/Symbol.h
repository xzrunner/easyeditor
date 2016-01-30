#ifndef _EASYTEXTURE_SYMBOL_H_
#define _EASYTEXTURE_SYMBOL_H_

#include <drag2d.h>

namespace etexture
{

class Symbol : public d2d::Symbol
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
	virtual void ReloadTexture() const;
	virtual void Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color = d2d::ColorTrans(), 
		const d2d::Sprite* spr = NULL, const d2d::Sprite* root = NULL) const;
	virtual d2d::Rect GetSize(const d2d::Sprite* sprite = NULL) const;

	const std::vector<d2d::Shape*>& GetAllShapes() const { return m_shapes; }

	static d2d::Symbol* Create() { return new Symbol(); }
	
protected:
	virtual void LoadResources();

	void Clear();

private:
	std::vector<d2d::Shape*> m_shapes;

	friend class SymbolContainer;

}; // Symbol

}

#endif // _EASYTEXTURE_SYMBOL_H_