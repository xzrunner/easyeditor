#ifndef _EASYTRAIL_SYMBOL_H_
#define _EASYTRAIL_SYMBOL_H_

#include <ee/Symbol.h>

struct t2d_emitter_cfg;
struct t2d_emitter;

namespace etrail
{

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
	virtual sm::rect GetSize(const ee::Sprite* sprite = NULL) const;

	const t2d_emitter_cfg* GetEmitterCfg() const { return m_et_cfg; }
	t2d_emitter* GetEmitter() { return m_et; }

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	t2d_emitter_cfg* m_et_cfg;

	t2d_emitter* m_et;

}; // Symbol

}

#endif // _EASYTRAIL_SYMBOL_H_