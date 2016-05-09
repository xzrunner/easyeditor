#ifndef _EASYEDITOR_PSEUDO3D_CAMERA_H_
#define _EASYEDITOR_PSEUDO3D_CAMERA_H_

#include "Camera.h"

#include <SM_Vector.h>

union sm_mat4;
struct sm_vec3;
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

	virtual sm::vec2 TransPosScreenToProject(int x, int y, int width, int height) const;
	virtual sm::vec2 TransPosProjectToScreen(const sm::vec2& proj, int width, int height) const;

	sm::vec2 TransPosProjectToScreen(const sm::vec3& proj, int width, int height) const;

	virtual void UpdateModelView() const;

	virtual float GetScale() const;
	virtual const sm::vec2& GetPosition() const;

	void Translate(const sm::vec3& offset);
	void Rotate(float da);

	float GetAngle() const;
	const sm_vec3* GetPos() const;

	const sm_mat4* GetModelViewMat() const;
	const sm_mat4* GetProjectMat() const;

private:
	c25_camera* m_cam;

}; // Pseudo3DCamera

}

#endif // _EASYEDITOR_PSEUDO3D_CAMERA_H_