#ifndef _EASYPARTICLE2D_SYMBOL_H_
#define _EASYPARTICLE2D_SYMBOL_H_

#include <ee/Symbol.h>

struct p2d_emitter_cfg;

namespace eparticle2d
{

class ParticleSystem;

class Symbol : public ee::Symbol
{
public:
	Symbol();
	Symbol(const Symbol& s);

	//
	// Cloneable interface
	//
	virtual Symbol* Clone() const;

	//
	// Symbol interfaces
	//
	virtual void Draw(const s2::RenderParams& params, const ee::Sprite* spr = NULL) const;
	virtual void ReloadTexture() const;
	virtual ee::Rect GetSize(const ee::Sprite* sprite = NULL) const;

	void SetEmitterCfg(p2d_emitter_cfg* cfg) { m_et_cfg = cfg; }
	const p2d_emitter_cfg* GetEmitterCfg() const { return m_et_cfg; }

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	p2d_emitter_cfg* m_et_cfg;

}; // ParticleSystem

}

#endif // _EASYPARTICLE2D_SYMBOL_H_
