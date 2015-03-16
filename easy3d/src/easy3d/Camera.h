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

	mat4 GetModelViewMat() const;

	const vec3& GetPosition() const { return m_pos; }
	void SetPosition(const vec3& pos) { m_pos = pos; }

	void Rotate(float dheading, float delevation);

	vec3 GetLeft() const;		// U
	vec3 GetUp() const;			// V
	vec3 GetToward() const;		// N

	void SetScreenSize(int width, int height);
	void GetScreenSize(int* width, int* height) const;

	vec3 MapToSphere(ivec2 touchpoint) const;

	void Reset();

public:
	static const float CAM_NEAR;
	static const float CAM_FAR;

private:
	mat4 GetModelViewRotMat() const;

private:
	vec3 m_pos;
	
	// use UVN model
	// in degree
	float m_rot_heading;	// ·½Î»½Ç
	float m_rot_elevation;	// Ñö½Ç

	int m_width, m_height;

}; // Camera

}

#endif // _E3D_CAMERA_H_