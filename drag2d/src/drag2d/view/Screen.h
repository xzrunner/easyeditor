#ifndef _D2D_SCREEN_H_
#define _D2D_SCREEN_H_

#include "common/Vector.h"

namespace d2d
{

class Screen
{
public:
	Screen() {}
	
	void SetSize(int w, int h, const Vector& offset = Vector(0,0), 
		const Vector& scale = Vector(1,1)) {
		m_size.x = w;
		m_size.y = h;
		m_offset = offset;
		m_scale = scale;
	}

	void TransPosForRender(Vector& pos) const {
		pos += m_offset;
		pos.x *= m_scale.x;
		pos.y *= m_scale.y;
		pos.x = pos.x * 2 / m_size.x;
		pos.y = pos.y * 2 / m_size.y;
	}

private:
	Vector m_size;
	Vector m_offset;
	Vector m_scale;

}; // Screen

}

#endif // _D2D_SCREEN_H_