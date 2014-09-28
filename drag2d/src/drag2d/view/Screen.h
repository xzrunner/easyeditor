#ifndef _D2D_SCREEN_H_
#define _D2D_SCREEN_H_

#include "common/Vector.h"

namespace d2d
{

class Camera;

class Screen
{
public:
//	Screen() : m_scale(1, 1) {}
	Screen(int width, int height) 
		: m_size(width, height) , m_scale(1, 1) {}
	Screen(Camera* cam) : m_cam(cam) {}

	const Vector& GetSize() const { return m_size; }
	void SetSize(int width, int height) { m_size.set(width, height); }

	void SetCamera(
		const Vector& offset = Vector(0,0), 
		const Vector& scale = Vector(1,1)) {
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

	const Vector& GetScale() const { return m_scale; }

	void UpdateModelView() const;

private:
	Vector m_size;
	Vector m_offset;
	Vector m_scale;

	Camera* m_cam;

}; // Screen

}

#endif // _D2D_SCREEN_H_