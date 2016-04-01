#ifndef _GLUE_SPRITE_H_
#define _GLUE_SPRITE_H_

#include "math/Vector.h"

#include <sm.h>

namespace glue
{

class Symbol;

class Sprite
{
public:
	Sprite(Symbol* sym);
	~Sprite();

	virtual void Update();

	const Symbol& GetSymbol() const { return *m_sym; }

	sm_mat4 GetTransMatrix() const;

	void SetPositioin(const vec2& pos) { m_pos = pos; }

private:
	vec2 m_pos;

	Symbol* m_sym;

}; // Sprite

}

#endif // _GLUE_SPRITE_H_