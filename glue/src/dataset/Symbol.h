#ifndef _GLUE_SYMBOL_H_
#define _GLUE_SYMBOL_H_

#include "math/Matrix.h"

namespace glue
{

class Symbol
{
public:
	virtual ~Symbol() {}
	virtual void Draw(const mat4& mt) const = 0;

}; // Symbol

}

#endif // _GLUE_SYMBOL_H_