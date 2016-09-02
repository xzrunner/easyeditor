#ifndef _SPRITE2_PROXY_SPRITE_H_
#define _SPRITE2_PROXY_SPRITE_H_

#include "S2_Sprite.h"

namespace s2
{

class ProxySprite : public VIRTUAL_INHERITANCE Sprite
{
public:
	ProxySprite();
	ProxySprite(Symbol* sym);

	/**
	 *  @interface
	 *    Cloneable
	 */
	virtual ProxySprite* Clone() const;

	static bool IsProxyName(const std::string& name);

}; // ProxySprite

}

#endif // _SPRITE2_PROXY_SPRITE_H_