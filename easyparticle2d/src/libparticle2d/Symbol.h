#ifndef _EASYPARTICLE2D_SYMBOL_H_
#define _EASYPARTICLE2D_SYMBOL_H_

#include <drag2d.h>

struct p2d_emitter_cfg;

namespace eparticle2d
{

class ParticleSystem;

class Symbol : public d2d::ISymbol
{
public:
	Symbol();
	Symbol(const Symbol& s);

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

	void SetEmitterCfg(p2d_emitter_cfg* cfg) { m_et_cfg = cfg; }
	const p2d_emitter_cfg* GetEmitterCfg() const { return m_et_cfg; }

	static d2d::ISymbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	p2d_emitter_cfg* m_et_cfg;

}; // ParticleSystem

}

#endif // _EASYPARTICLE2D_SYMBOL_H_
