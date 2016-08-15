#ifndef _EASYEJOY2D_SYMBOL_H_
#define _EASYEJOY2D_SYMBOL_H_

#include <ee/Symbol.h>

#include <sprite2/DummySymbol.h>

namespace eejoy2d
{

class EJSprite;

class Symbol : public ee::Symbol, public s2::DummySymbol
{
public:
	Symbol();
	Symbol(const Symbol& s);
	virtual ~Symbol();

	/**
	 *  @interface
	 *    s2::Symbol
	 */
	virtual void Draw(const s2::RenderParams& params, const s2::Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const s2::Sprite* sprite = NULL) const;

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