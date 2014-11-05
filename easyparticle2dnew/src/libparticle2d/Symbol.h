#ifndef _EASYPARTICLE2D_SYMBOL_H_
#define _EASYPARTICLE2D_SYMBOL_H_

#include <drag2d.h>

namespace eparticle2d
{

class ParticleSystem;

class Symbol : public d2d::ISymbol
{
public:
	Symbol();
	Symbol(const Symbol& s);
	virtual ~Symbol();

	//
	// ICloneable interface
	//
	virtual Symbol* clone() const;

	//
	// ISymbol interfaces
	//
	virtual void reloadTexture() const;
	virtual void draw(const d2d::Matrix& mt,
		const d2d::Colorf& mul = d2d::Colorf(1, 1, 1, 1), 
		const d2d::Colorf& add = d2d::Colorf(0, 0, 0, 0),
		const d2d::ISprite* sprite = NULL) const;
	virtual d2d::Rect getSize(const d2d::ISprite* sprite = NULL) const;

	static d2d::ISymbol* Create() { return new Symbol(); }

	void ResetPS();

protected:
	virtual void loadResources();

private:
	void DrawBackground(const d2d::Vector& pos) const;

private:
	ParticleSystem* m_ps;

	d2d::Rect m_region;

}; // ParticleSystem

}

#endif // _EASYPARTICLE2D_SYMBOL_H_
