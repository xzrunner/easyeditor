#ifndef _SPRITE2_TEXCOORDS_MAP_H_
#define _SPRITE2_TEXCOORDS_MAP_H_

#include <SM_Vector.h>

namespace s2
{

class TexcoordsMap
{
public:
	static void Trans(float src[8], sm::vec2 dst[4]);

}; // TexcoordsMap

}

#endif // _SPRITE2_TEXCOORDS_MAP_H_