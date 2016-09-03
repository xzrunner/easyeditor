#ifndef _GLUE_SPRITE_FACTORY_H_
#define _GLUE_SPRITE_FACTORY_H_

#include <CU_Singleton.h>

namespace s2 { class Symbol; }

namespace glue
{

class SpriteFactory
{
public:
	s2::Sprite* Create() const;

private:

	SINGLETON_DECLARATION(SpriteFactory);

}; // SpriteFactory

}

#endif // _GLUE_SPRITE_FACTORY_H_