#ifndef _EASYEJOY2D_SYMBOL_H_
#define _EASYEJOY2D_SYMBOL_H_

#include <ee/Symbol.h>

namespace eejoy2d
{

class EJSprite;

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
	virtual void Draw(const s2::RenderParams& params, const ee::Sprite* spr = NULL) const;
	virtual sm::rect GetSize(const ee::Sprite* sprite = NULL) const;

	static ee::Symbol* Create() { return new Symbol(); }

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