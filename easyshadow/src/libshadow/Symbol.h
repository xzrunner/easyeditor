#ifndef _EASYSHADOW_SYMBOL_H_
#define _EASYSHADOW_SYMBOL_H_

#include <drag2d.h>

namespace eshadow
{

class Shadow;

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

	Shadow* GetShadow() { return m_shadow; }
	const Shadow* GetShadow() const { return m_shadow; }

	static d2d::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	Shadow* m_shadow;

}; // Symbol

}

#endif // _EASYSHADOW_SYMBOL_H_