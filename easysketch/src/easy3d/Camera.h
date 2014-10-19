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

	void SetRotate(const mat4& rot);

	mat4 GetMatrix() const;

	void SetScreenSize(int width, int height);

	vec3 MapToSphere(ivec2 touchpoint) const;

	static float GetNear();
	static float GetFar();

private:
	mat4 m_translation;
	mat4 m_rotation;

	int m_width, m_height;

}; // Camera

}

#endif // _E3D_CAMERA_H_