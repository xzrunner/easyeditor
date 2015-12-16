#ifndef _EASYPARTICLE3D_SYMBOL_H_
#define _EASYPARTICLE3D_SYMBOL_H_

#include <drag2d.h>

namespace eparticle3d
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
	virtual Symbol* Clone() const;

	//
	// ISymbol interfaces
	//
	virtual void ReloadTexture() const;
	virtual void Draw(const d2d::Matrix& mt, const d2d::ColorTrans& color = d2d::ColorTrans(), 
		const d2d::ISprite* spr = NULL, const d2d::ISprite* root = NULL) const;
	virtual d2d::Rect GetSize(const d2d::ISprite* sprite = NULL) const;

	void Start() const;

	ParticleSystem* GetPS() { return m_ps; }
	const ParticleSystem* GetPS() const { return m_ps; }

	void SetPS(ParticleSystem* ps);

	static d2d::ISymbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	ParticleSystem* m_ps;

	d2d::Rect m_region;

	mutable clock_t m_time;

}; // ParticleSystem

}

#endif // _EASYPARTICLE3D_SYMBOL_H_
