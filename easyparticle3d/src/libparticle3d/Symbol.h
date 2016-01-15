#ifndef _EASYPARTICLE3D_SYMBOL_H_
#define _EASYPARTICLE3D_SYMBOL_H_

#include <drag2d.h>

struct p3d_emitter_cfg;
struct p3d_emitter;

namespace eparticle3d
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

	void SetEmitterCfg(p3d_emitter_cfg* cfg) { m_et_cfg = cfg; }
	const p3d_emitter_cfg* GetEmitterCfg() const { return m_et_cfg; }

	p3d_emitter* GetEmitter() { return m_et; }

	static d2d::ISymbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	p3d_emitter_cfg* m_et_cfg;

	p3d_emitter* m_et;
	
}; // ParticleSystem

}

#endif // _EASYPARTICLE3D_SYMBOL_H_
