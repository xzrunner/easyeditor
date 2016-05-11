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

	//
	// Cloneable interface
	//
	virtual Symbol* Clone() const { return NULL; }

	//
	// Symbol interfaces
	//
	virtual void Draw(const s2::RenderParams& params, const ee::Sprite* spr = NULL) const;
	virtual ee::Rect GetSize(const ee::Sprite* sprite = NULL) const;

	static ee::Symbol* Create() { return new Symbol(); }

protected:
	virtual void LoadResources();

private:
	ParticleSystem* m_ps;

}; // Symbol

}

#endif // _EASYP3DINV_SYMBOL_H_