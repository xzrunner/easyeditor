#ifndef _D2D_SCREEN_H_
#define _D2D_SCREEN_H_

#include "common/Vector.h"

namespace d2d
{

class Screen
{
public:
	Screen() : m_scale(1) {}
	
	void SetSize(int w, int h, const Vector& offset, float scale) {
		m_size.x = w;
		m_size.y = h;
		m_offset = offset;
		m_scale = scale;
	}

	void TransPosForRender(Vector& pos) const {
		pos += m_offset;
		pos *= m_scale;
		pos.x = pos.x * 2 / m_size.x;
		pos.y = pos.y * 2 / m_size.y;
	}

private:
	Vector m_size;
	Vector m_offset;
	float m_scale;

}; // Screen

}

#endif // _D2D_SCREEN_H_