#ifndef _EASYP3DINV_SYMBOL_H_
#define _EASYP3DINV_SYMBOL_H_

#include <ee/Symbol.h>

namespace ep3dinv
{

class ParticleSystem;

class Symbol : public ee::Symbol
{
public:
	Symbol();
	virtual ~Symbol();

	/**
	 *  @interface
	 *    s2::Symbol
	 */
	virtual void Draw(const s2::RenderParams& params, const s2::Sprite* spr = NULL) const;

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual sm::rect GetBoundingImpl(const s2::Sprite* spr = NULL, const s2::Actor* actor = NULL, bool cache = true) const;

	virtual bool LoadResources();

private:
	ParticleSystem* m_ps;

}; // Symbol

}

#endif // _EASYP3DINV_SYMBOL_H_