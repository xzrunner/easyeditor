#ifndef _SPRITE2_TRAIL_SYMBOL_H_
#define _SPRITE2_TRAIL_SYMBOL_H_

#include "S2_Symbol.h"

struct t2d_emitter_cfg;
struct t2d_emitter;

namespace s2
{

class TrailSymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	TrailSymbol();
	virtual ~TrailSymbol();

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const Sprite* spr = NULL) const;

	void SetEmitterCfg(t2d_emitter_cfg* cfg);
	const t2d_emitter_cfg* GetEmitterCfg() const { return m_et_cfg; }

	const t2d_emitter* GetEmitter() const { return m_et; }

protected:
	t2d_emitter_cfg* m_et_cfg;

	t2d_emitter* m_et;

}; // TrailSymbol

}

#endif // _SPRITE2_TRAIL_SYMBOL_H_