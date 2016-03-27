#ifndef _EASYEDITOR_SCREEN_H_
#define _EASYEDITOR_SCREEN_H_

#include "Vector.h"
#include "Rect.h"

namespace ee
{

class Screen
{
public:
	Screen() {}

	const Vector& GetSize() const { return m_size; }
	void SetSize(int width, int height) { m_size.Set(static_cast<float>(width), static_cast<float>(height)); }

	void SetCamera(
		const Vector& offset = Vector(0,0), 
		const Vector& scale = Vector(1,1)) {
		m_offset = offset;
		m_scale = scale;
	}

// 	void TransPosForRender(Vector& pos) const {
// 		pos += m_offset;
// 		pos.x *= m_scale.x;
// 		pos.y *= m_scale.y;
// 		pos.x = pos.x * 2 / m_size.x;
// 		pos.y = pos.y * 2 / m_size.y;
// 	}

	float GetScale() const;

	void UpdateModelView() const;

	Rect GetRegion() const;

private:
	Vector m_size;
	Vector m_offset;
	Vector m_scale;

}; // Screen

}

#endif // _EASYEDITOR_SCREEN_H_