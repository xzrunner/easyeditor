#ifndef _E3D_CAMERA_H_
#define _E3D_CAMERA_H_

#include "Vector.h"
#include "Matrix.h"

namespace e3d
{

class Camera
{
public:
	Camera();

	void Translate(const vec3& offset);
	void Zoom(bool zoomin);

	mat4 GetMatrix() const;

	const vec3& GetPosition() const { return m_pos; }
	void SetPosition(const vec3& pos) { m_pos = pos; }

	const mat4& GetRotate() const { return m_rotation; }
	void SetRotate(const mat4& rot);

	void SetScreenSize(int width, int height);

	vec3 MapToSphere(ivec2 touchpoint) const;

	void Reset();

public:
	static const float CAM_NEAR;
	static const float CAM_FAR;

private:
	vec3 m_pos;
	mat4 m_rotation;

	int m_width, m_height;

}; // Camera

}

#endif // _E3D_CAMERA_H_