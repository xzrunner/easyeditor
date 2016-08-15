#ifndef _EASYPARTICLE2D_SYMBOL_H_
#define _EASYPARTICLE2D_SYMBOL_H_

#include <ee/Symbol.h>

#include <sprite2/Particle2dSymbol.h>

struct p2d_emitter_cfg;

namespace eparticle2d
{

class ParticleSystem;

class Symbol : public ee::Symbol, public s2::Particle2dSymbol
{
public:
	Symbol();
	Symbol(const Symbol& s);

	/**
	 *  @interface
	 *    s2::Symbol
	 */
	virtual void Draw(const s2::RenderParams& params, const s2::Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const s2::Sprite* sprite = NULL) const;

	/**
	 *  @interface
	 *    ee::Symbol
	 */
	virtual void ReloadTexture() const;

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
