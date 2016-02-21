#ifndef _EASY3D_CAMERA_H_
#define _EASY3D_CAMERA_H_

#include <ee/Vector3D.h>
#include <ee/Matrix3D.h>

namespace e3d
{

class Camera
{
public:
	Camera();

	void Translate(const ee::vec3& offset);
	void Zoom(bool zoomin);

	ee::mat4 GetModelViewMat() const;

	const ee::vec3& GetPosition() const { return m_pos; }
	void SetPosition(const ee::vec3& pos) { m_pos = pos; }

	float GetRotHeading() const { return m_rot_heading; }
	void SetRotHeading(float rot_heading) { m_rot_heading = rot_heading; }
	float GetRotElevation() const { return m_rot_elevation; }
	void SetRotElevation(float rot_elevation) { m_rot_elevation = rot_elevation; }

	void Rotate(float dheading, float delevation);

	ee::vec3 GetLeft() const;		// U
	ee::vec3 GetUp() const;			// V
	ee::vec3 GetToward() const;		// N

	void SetScreenSize(int width, int height);

	ee::vec3 MapToSphere(ee::ivec2 touchpoint) const;

	void Reset();

public:
	static const float CAM_NEAR;
	static const float CAM_FAR;

private:
	ee::mat4 GetModelViewRotMat() const;

private:
	ee::vec3 m_pos;
	
	// use UVN model
	// in degree
	float m_rot_heading;	// ·½Î»½Ç
	float m_rot_elevation;	// Ñö½Ç

	int m_width, m_height;

}; // Camera

}

#endif // _EASY3D_CAMERA_H_