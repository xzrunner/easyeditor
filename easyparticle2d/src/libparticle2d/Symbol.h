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
	virtual Symbol* Clone() const;

	//
	// ISymbol interfaces
	//
	virtual void ReloadTexture() const;
	virtual void Draw(const d2d::Matrix& mt,
		const d2d::Colorf& mul = d2d::Colorf(1, 1, 1, 1), 
		const d2d::Colorf& add = d2d::Colorf(0, 0, 0, 0),
		const d2d::Colorf& r_trans = d2d::Colorf(1, 0, 0, 0),
		const d2d::Colorf& g_trans = d2d::Colorf(0, 1, 0, 0),
		const d2d::Colorf& b_trans = d2d::Colorf(0, 0, 1, 0),
		const d2d::ISprite* sprite = NULL) const;
	virtual d2d::Rect GetSize(const d2d::ISprite* sprite = NULL) const;

	void ResetPS();

	ParticleSystem* GetPS() { return m_ps; }
	const ParticleSystem* GetPS() const { return m_ps; }

	void SetPS(ParticleSystem* ps);

	static d2d::ISymbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	void DrawPS(const d2d::Matrix& mt) const;

	void DrawBackground(const d2d::Vector& pos) const;

private:
	ParticleSystem* m_ps;

	d2d::Rect m_region;

	int m_curr_frame;

}; // ParticleSystem

}

#endif // _EASYPARTICLE2D_SYMBOL_H_
