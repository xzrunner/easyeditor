#ifndef _GLUE_SYMBOL_H_
#define _GLUE_SYMBOL_H_

union sm_mat4;

namespace glue
{

class Symbol
{
public:
	virtual ~Symbol() {}
	virtual void Draw(const sm_mat4& mt) const = 0;

}; // Symbol

}

#endif // _GLUE_SYMBOL_H_