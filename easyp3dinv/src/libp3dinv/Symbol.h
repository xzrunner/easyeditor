#ifndef _EASYP3DINV_SYMBOL_H_
#define _EASYP3DINV_SYMBOL_H_

#include <drag2d.h>

namespace ep3dinv
{

class ParticleSystem;

class Symbol : public d2d::Symbol
{
public:
	Symbol();
	virtual ~Symbol();

	//
	// Cloneable interface
	//
	virtual Symbol* Clone() const { return NULL; }

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
	ParticleSystem* m_ps;

}; // Symbol

}

#endif // _EASYP3DINV_SYMBOL_H_