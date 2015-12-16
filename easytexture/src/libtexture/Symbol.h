#ifndef _EASYTEXTURE_SYMBOL_H_
#define _EASYTEXTURE_SYMBOL_H_

#include <drag2d.h>

namespace etexture
{

class Symbol : public d2d::ISymbol
{
public:
	Symbol();
	Symbol(const Symbol& s);
	virtual ~Symbol();

	//
	// ICloneable interface
	//
	virtual Symbol* Clone() const;

	//
	// ISymbol interfaces
	//
	virtual void ReloadTexture() const;
	virtual void Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color = d2d::ColorTrans(), 
		const d2d::ISprite* spr = NULL, const d2d::ISprite* root = NULL) const;
	virtual d2d::Rect GetSize(const d2d::ISprite* sprite = NULL) const;

	const std::vector<d2d::IShape*>& GetAllShapes() const { return m_shapes; }

	static d2d::ISymbol* Create() { return new Symbol(); }
	
protected:
	virtual void LoadResources();

	void Clear();

private:
	std::vector<d2d::IShape*> m_shapes;

	friend class SymbolContainer;

}; // Symbol

}

#endif // _EASYTEXTURE_SYMBOL_H_