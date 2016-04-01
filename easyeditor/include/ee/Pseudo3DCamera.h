#ifndef _EASYEDITOR_PSEUDO3D_CAMERA_H_
#define _EASYEDITOR_PSEUDO3D_CAMERA_H_

#include "Camera.h"
#include "Vector3D.h"

union sm_mat4;
struct c25_camera;

namespace ee
{

class Pseudo3DCamera : public Camera
{
public:
	Pseudo3DCamera();
	~Pseudo3DCamera();

	virtual std::string Type() const { return "pseudo3d"; }
	virtual void Reset();

	virtual Vector TransPosScreenToProject(int x, int y, int width, int height) const;
	virtual Vector TransPosProjectToScreen(const Vector& proj, int width, int height) const;

	Vector TransPosProjectToScreen(const vec3& proj, int width, int height) const;

	virtual void UpdateModelView() const;

	virtual float GetScale() const;
	virtual const Vector& GetPosition() const;

	void Translate(const vec3& offset);
	void Rotate(float da);

	float GetAngle() const;
	float GetZ() const;

public:
	static const int FAR_CLIP = 800;
	static const int NEAR_CLIP = 50;

private:
	c25_camera* m_cam;

}; // Pseudo3DCamera

}

#endif // _EASYEDITOR_PSEUDO3D_CAMERA_H_