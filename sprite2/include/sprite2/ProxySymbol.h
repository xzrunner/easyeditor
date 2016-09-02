#ifndef _SPRITE2_PROXY_SYMBOL_H_
#define _SPRITE2_PROXY_SYMBOL_H_

#include "S2_Symbol.h"

namespace s2
{

class ProxySymbol : public VIRTUAL_INHERITANCE Symbol
{
public:
	ProxySymbol();
	virtual ~ProxySymbol();

	/**
	 *  @interface
	 *    Symbol
	 */
	virtual void Draw(const RenderParams& params, const Sprite* spr = NULL) const;
	virtual sm::rect GetBounding(const Sprite* spr = NULL) const;

	Sprite* GetReal() { return m_spr; }
	
private:
	Sprite* m_spr;

}; // ProxySymbol

}

#endif // _SPRITE2_PROXY_SYMBOL_H_