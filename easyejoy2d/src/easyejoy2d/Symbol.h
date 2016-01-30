#ifndef _EASYEJOY2D_SYMBOL_H_
#define _EASYEJOY2D_SYMBOL_H_

#include <drag2d.h>

namespace eejoy2d
{

class EJSprite;

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

	static d2d::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	void DrawToEJScreen() const;
	void DrawFromEJScreen() const;

private:
	EJSprite* m_spr;

}; // Symbol

}

#endif // _EASYEJOY2D_SYMBOL_H_