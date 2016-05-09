#ifndef _EASYEDITOR_SCREEN_H_
#define _EASYEDITOR_SCREEN_H_

#include "Rect.h"

namespace ee
{

class Screen
{
public:
	Screen() {}

	const sm::vec2& GetSize() const { return m_size; }
	void SetSize(int width, int height) { m_size.Set(static_cast<float>(width), static_cast<float>(height)); }

	void SetCamera(
		const sm::vec2& offset = sm::vec2(0,0), 
		const sm::vec2& scale = sm::vec2(1,1)) {
		m_offset = offset;
		m_scale = scale;
	}

// 	void TransPosForRender(sm::vec2& pos) const {
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
	sm::vec2 m_size;
	sm::vec2 m_offset;
	sm::vec2 m_scale;

}; // Screen

}

#endif // _EASYEDITOR_SCREEN_H_